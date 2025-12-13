/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "stdint.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef struct Task_Control_Block{
  volatile uint32_t *stackPtr;
} OS_TCB;

OS_TCB Task1TCB;

uint32_t Task1stack[128];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
__asm int asm_add(int a, int b)
{
  ADD R0, R0, R1 
  BX LR 
}

__asm uint32_t get_msp_val()
{
  MRS R0, MSP
  BX LR
}

__asm uint32_t get_psp_val()
{
  MRS R0, PSP
  BX LR
}

__asm void save_context_test()
{
  MRS R0, PSP
  STMDB R0!, {R4-R11}
  MOV R4, R0
  BX LR
}

__asm void setup_fake_environment(void)
{
    // 【造假数据】强行把 R4-R11 填满记号
    MOV R4, #0x04040404
    MOV R5, #0x05050505
    MOV R6, #0x06060606
    MOV R7, #0x07070707
    MOV R8, #0x08080808
    MOV R9, #0x09090909
    MOV R10, #0x10101010
    MOV R11, #0x11111111

    // 【设置PSP】这里可以用 LDR 伪指令了，汇编器能看懂
    LDR R0, =0x20001000
    MSR PSP, R0

    // 回家
    BX LR
}

__asm void set_registers(void) 
{
  MOV R0, #0X11111111
  MOV R1, #0X22222222
  MOV R2, #0X33333333
  MOV R3, #0X44444444

  BX LR
}

__asm void create_fake_task()
{
  MOV R0, #0x20002000
  MSR PSP, R0
  MOV R0, #2
  MSR CONTROL, R0
  ISB
  BX LR
}

void Task_End_Hook()
{
  for(;;);
}

uint32_t* Task_Stack_Init(void* task_function, uint32_t* stack_init_address, uint32_t stack_depth)
{
  /* 第一步：找到栈顶 */
  uint32_t *sp = stack_init_address + stack_depth;

  /* 第二步：字节对齐 */
  sp = (uint32_t *)((uint32_t)sp & 0xFFFFFFF8); // 先把sp转成uint32_t，再把最后3位抹成0，最后转成uint32_t *

  /* 第三步：填入数据 */
  /* 硬件区 */
  *(--sp) = (uint32_t)0x01000000; // xPSR
  *(--sp) = (uint32_t)task_function; // PC
  *(--sp) = (uint32_t)Task_End_Hook; // LR
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

void Task1()
{
  for(;;);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  Task1TCB.stackPtr = Task_Stack_Init(Task1, Task1stack, 128);
	__asm("ADD R0, R0, R1 \n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {  

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
