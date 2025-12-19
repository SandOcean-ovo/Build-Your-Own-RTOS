/**
 ******************************************************************************
 * @file    os_core.c
 * @author  SandOcean
 * @version V1.0
 * @date    2025-12-14
 * @brief   RTOS 内核核心逻辑实现
 *
 * 本文件包含 RTOS 的独立于硬件的逻辑实现：
 * - OS 初始化与启动逻辑 (OS_Init, OS_Start)
 * - 任务调度器算法 (Scheduler) 实现
 * - SysTick 时钟节拍处理 (Timebase management)
 * - 阻塞延时处理 (osDelay) 与就绪表管理
 *
 ******************************************************************************
 */

#include "os_core.h"

/* 宏定义 ----------------------------------------------------------- */

#define IDLE_STACK_SIZE 128

/* 私有变量定义 ------------------------------------------------------ */

volatile uint32_t g_SystemTickCount = 0; // 系统心跳计数器

OS_TCB* CurrentTCB = NULL;
OS_TCB* NextTCB = NULL;

OS_TCB IdleTaskTCB;
uint32_t IdleTaskStack[IDLE_STACK_SIZE]; 

/* 私有函数定义 ------------------------------------------------------ */
void IdleTask(void)
{
    for(;;);
}

OS_TCB* FindNextTask(void)
{
    // 1. 从当前任务的下一个开始找
    OS_TCB* TempTCB = CurrentTCB->Next; 

    // 2. 如果该任务在睡觉，就找【它】的下一个
    // 注意：这里用 TempTCB->Next，而不是 CurrentTCB->Next
    while (TempTCB->DelayTicks > 0)
    {
        TempTCB = TempTCB->Next; 
    }

    return TempTCB;
}

/* 函数声明 ----------------------------------------------------------- */

void OS_TaskCreate(OS_TCB* tcb, void* task_function, uint32_t* stack_init_address, uint32_t stack_depth)
{
    tcb->stackPtr = OS_StackInit(task_function, stack_init_address, stack_depth);

    tcb->DelayTicks = 0;

    if(CurrentTCB == NULL){
        tcb->Next = tcb;
        CurrentTCB = tcb;
    }
    else{
        tcb->Next = CurrentTCB->Next;
        CurrentTCB->Next = tcb;
    }
}

void OS_StartScheduler(void)
{

    // 0. 创建空闲任务 确保系统中至少有一个始终处于就绪态的任务
    OS_TaskCreate(&IdleTaskTCB, IdleTask, IdleTaskStack, IDLE_STACK_SIZE);

    // 1. 此时 CurrentTCB 肯定不为 NULL 了，原来的防御性检查可以删除
    // if (CurrentTCB == NULL) return; 

    // 2. 关键步骤：设置 NextTCB 为第一个要运行的任务
    NextTCB = CurrentTCB;

    // 3. 关键步骤：欺骗 PendSV
    // 将 CurrentTCB 暂时设为 NULL。
    // 这样 PendSV 里的 "CMP R1, #0" 就会成立，从而跳过 STMDB (保存上下文)，
    // 直接执行 RestoreContext (恢复 NextTCB 的上下文)。
    CurrentTCB = NULL;

    // 4. 初始化 SysTick (开启时间片，开始 1ms 中断)
    // 注意：SysTick_Handler 里有一句 if(CurrentTCB != NULL)，
    // 所以在 PendSV 执行完之前，SysTick 即使触发了也不会乱调度。
    OS_Init_Timer(1); 

    // 5. 触发 PendSV，开始第一次切换！
    OS_Trigger_PendSV();

    // 6. 应该永远不会执行到这里
    while(1);
}

void OS_Tick_Handler(void)
{
    // 1. 安全检查
    if (CurrentTCB == NULL) return;

    // 2. 更新系统时间
    g_SystemTickCount++;

    // 3. 遍历任务列表，将每个延时值（若>0）-1
    OS_TCB* ptr = CurrentTCB; // 从当前任务开始

    do{
        if(ptr->DelayTicks > 0) ptr->DelayTicks--;
        ptr = ptr->Next; // 将当前标记指针指向下一个任务
    }while(ptr != CurrentTCB);

    // 4. 核心调度逻辑
    NextTCB = FindNextTask();

    // 5. 请求上下文切换
    // 这里不再直接写寄存器，而是调用移植层的接口
    if (NextTCB != CurrentTCB) {
        OS_Trigger_PendSV();
    }
}

void OS_Delay(uint32_t ticks)
{
    OS_Disable_IRQ();
    CurrentTCB->DelayTicks = ticks;
    OS_Enable_IRQ();

    NextTCB = FindNextTask();
    
    OS_Trigger_PendSV();
}

