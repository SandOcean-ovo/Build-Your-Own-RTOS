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

/* 函数声明 ---------------------------------------------------------------- */

/** 
 * @brief 任务栈初始化 
 */
uint32_t* Task_Stack_Init(void* task_function, uint32_t* stack_init_address, uint32_t stack_depth);

/** 
 * @brief 任务异常结束钩子函数
 */
void Task_End_Hook(void);

#endif /* __OS_CPU_H */
