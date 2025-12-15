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
  *(--sp) = (uint32_t)0x12121212; // R12 
  *(--sp) = (uint32_t)0x03030303; // R3
  *(--sp) = (uint32_t)0x02020202; // R2
  *(--sp) = (uint32_t)0x01010101; // R1
  *(--sp) = (uint32_t)0x00000000; // R0

  /* 软件区 */
  *(--sp) = (uint32_t)0x11111111; // R11
  *(--sp) = (uint32_t)0x10101010; // R10
  *(--sp) = (uint32_t)0x09090909; // R9
  *(--sp) = (uint32_t)0x08080808; // R8
  *(--sp) = (uint32_t)0x07070707; // R7
  *(--sp) = (uint32_t)0x06060606; // R6
  *(--sp) = (uint32_t)0x05050505; // R5
  *(--sp) = (uint32_t)0x04040404; // R4

  /* 第四步：返回sp */
  return sp;
}


