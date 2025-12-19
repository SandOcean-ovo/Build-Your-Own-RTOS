/**
 ******************************************************************************
 * @file    os_core.h
 * @author  SandOcean
 * @version V1.0
 * @date    2025-12-14
 * @brief   RTOS 核心内核头文件 (Kernel API)
 *
 * 本文件包含 RTOS 内核的核心定义与对外接口声明：
 * - 任务控制块 (TCB) 结构体定义
 * - 任务创建与堆栈初始化函数声明
 * - 核心调度器 (Scheduler) 与上下文切换接口
 * - 延时函数 (osDelay) 与时基管理
 *
 ******************************************************************************
 */

#ifndef __OS_CORE_H
#define __OS_CORE_H

#include "os_cpu.h"
#include "os_types.h"
#include <stddef.h>

/* 数据结构定义 -------------------------------------------------------- */

/**
 * @brief  任务状态枚举 
 */
typedef enum {
    TASK_READY = 0,  ///< 就绪：随时可以跑
    TASK_BLOCKED,    ///< 阻塞：在等时间，或者等信号量
} OS_TaskState;
/**
 * @brief  任务控制块结构体定义 
 */
typedef struct Task_Control_Block
{
    volatile uint32_t *stackPtr; ///< 任务对应的栈指针
    struct Task_Control_Block *Next; ///< 指向下一个任务的指针
    OS_TaskState State; ///< 任务状态
    volatile uint32_t DelayTicks; ///< 延时的时间（单位ms）
    struct Task_Control_Block *NextWaitTask ///< 指向下一个正在等待同一个信号量的任务
} OS_TCB;

/**
 * @brief  信号量结构体定义 
 */
typedef struct Semaphore
{
    volatile uint16_t count;
    OS_TCB  *WaitListHead; 
    OS_TCB  *WaitListTail;
} OS_Sem;


/* 全局变量声明 -------------------------------------------------------- */
extern volatile uint32_t g_SystemTickCount;
extern OS_TCB* task_list_head;
extern OS_TCB* CurrentTCB;
extern OS_TCB* NextTCB;

/* 函数声明 ----------------------------------------------------------- */

/**
 * @brief  新建任务
 * @param  tcb: 任务对应的任务控制块指针
 * @param  task_entry: 任务入口函数地址
 * @param  stack_top : 栈数组的起始地址（低地址）
 * @param  stack_size: 栈大小（单位：元素个数，不是字节）
 */
void OS_TaskCreate(OS_TCB* tcb, void* task_function, uint32_t* stack_init_address, uint32_t stack_depth);

/**
 * @brief  开启调度器
 */
void OS_StartScheduler(void);

/**
 * @brief  处理SysTick中断的“回调函数”
 */
void OS_Tick_Handler(void);

/**
 * @brief  任务阻塞延时
 * @param  ticks: 延时的时间长度（单位ms）
 */
void OS_Delay(uint32_t ticks);

/**
 * @brief  进入临界区
 */
void OS_EnterCritical(void);

/**
 * @brief  退出临界区
 */
void OS_ExitCritical(void);

/**
 * @brief  等待信号量
 * @param  p_sem: 指向信号量的指针变量
 * @return uint8_t: 只会返回 1，代表接收到信号量
 */
uint8_t OS_SemWait(OS_Sem *p_sem);

/**
 * @brief  发送信号量
 * @param  p_sem: 指向信号量的指针变量
 * @return uint8_t: 只会返回 1，代表发送出信号量
 */
uint8_t OS_SemPost(OS_Sem *p_sem);

#endif /* __OS_CORE_H */
