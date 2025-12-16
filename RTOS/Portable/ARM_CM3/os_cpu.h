/**
 ******************************************************************************
 * @file    os_cpu.h
 * @author  SandOcean
 * @version V1.0
 * @date    2025-12-14
 * @brief   RTOS 架构相关头文件 (ARM Cortex-M3)
 *
 * 本文件包含与特定硬件架构相关的定义和宏：
 * - 处理器特定的数据类型
 * - 临界区保护宏 (关中断/开中断)
 * - 堆栈增长方向定义
 * - 汇编指令封装
 *
 ******************************************************************************
 */

#ifndef __OS_CPU_H
#define __OS_CPU_H

#include <stdint.h>
#include "stm32f1xx.h"

/* 函数声明 ---------------------------------------------------------------- */

/**
 * @brief  初始化任务栈
 * @param  task_entry: 任务入口函数地址
 * @param  stack_top : 栈数组的起始地址（低地址）
 * @param  stack_size: 栈大小（单位：元素个数，不是字节）
 * @return uint32_t* : 初始化后的栈顶指针 (SP)
 */
uint32_t* OS_StackInit(void* task_function, uint32_t* stack_init_address, uint32_t stack_depth);

/**
 * @brief  初始化SysTick
 * @param  ms: 时间片长度（单位ms） 
 */
void OS_Init_Timer(uint32_t ms);

/**
 * @brief  触发PendSV中断
 */
void OS_Trigger_PendSV(void);

#endif /* __OS_CPU_H */
