/**
 ******************************************************************************
 * @file    os_cpu.c
 * @author  SandOcean
 * @version V1.0
 * @date    2025-12-14
 * @brief   RTOS 移植层 C 语言实现 (ARM Cortex-M3)
 *
 * 本文件包含涉及硬件细节但可用 C 语言实现的函数：
 * - 任务栈初始化 (Task_Stack_Init)
 * - 伪造异常栈帧 (xPSR, PC, LR, R12, R3-R0)
 *
 ******************************************************************************
 */

#include "os_cpu.h"


void OS_TaskReturn(void)
{
  for(;;);
}

uint32_t* OS_StackInit(void* task_function, uint32_t* stack_init_address, uint32_t stack_depth)
{
  /* 第一步：找到栈顶 */
  uint32_t* sp = stack_init_address + stack_depth;

  /* 第二步：字节对齐 */
  sp = (uint32_t *)((uint32_t)sp & 0xFFFFFFF8); // 先把sp转成uint32_t，再把最后3位抹成0，最后转成uint32_t *

  /* 第三步：填入数据 */
  /* 硬件区 */
  *(--sp) = (uint32_t)0x01000000; // xPSR
  *(--sp) = (uint32_t)task_function; // PC
  *(--sp) = (uint32_t)OS_TaskReturn; // LR
  *(--sp) = (uint32_t)0x0; // R12 
  *(--sp) = (uint32_t)0x0; // R3
  *(--sp) = (uint32_t)0x0; // R2
  *(--sp) = (uint32_t)0x0; // R1
  *(--sp) = (uint32_t)0x0; // R0

  /* 软件区 */
  *(--sp) = (uint32_t)0x0; // R11
  *(--sp) = (uint32_t)0x0; // R10
  *(--sp) = (uint32_t)0x0; // R9
  *(--sp) = (uint32_t)0x0; // R8
  *(--sp) = (uint32_t)0x0; // R7
  *(--sp) = (uint32_t)0x0; // R6
  *(--sp) = (uint32_t)0x0; // R5
  *(--sp) = (uint32_t)0x0; // R4

  /* 第四步：返回sp */
  return sp;
}

void OS_Init_Timer(uint32_t ms)
{
    uint32_t ticks = 72000000 * ms / 1000;

    if(SysTick_Config(ticks)){
        while(1); /* 配置失败了，死循环 */
    }

    /* 设置优先级 */
    NVIC_SetPriority(PendSV_IRQn, 15); 
    
    NVIC_SetPriority(SysTick_IRQn, 14); 

    __enable_irq(); // 开全局中断
}

void OS_Trigger_PendSV(void)
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void OS_Enable_IRQ(void)
{
  __enable_irq();
}

void OS_Disable_IRQ(void)
{
  __disable_irq();
}
