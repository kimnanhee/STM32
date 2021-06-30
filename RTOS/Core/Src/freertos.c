/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>

#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for BlinkLED1 */
osThreadId_t BlinkLED1Handle;
const osThreadAttr_t BlinkLED1_attributes = {
  .name = "BlinkLED1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BlinkLED2 */
osThreadId_t BlinkLED2Handle;
const osThreadAttr_t BlinkLED2_attributes = {
  .name = "BlinkLED2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SendCds1 */
osThreadId_t SendCds1Handle;
const osThreadAttr_t SendCds1_attributes = {
  .name = "SendCds1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void LED1(void *argument);
void LED2(void *argument);
void CDS1(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of BlinkLED1 */
  BlinkLED1Handle = osThreadNew(LED1, NULL, &BlinkLED1_attributes);

  /* creation of BlinkLED2 */
  BlinkLED2Handle = osThreadNew(LED2, NULL, &BlinkLED2_attributes);

  /* creation of SendCds1 */
  SendCds1Handle = osThreadNew(CDS1, NULL, &SendCds1_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_LED1 */
/**
  * @brief  Function implementing the BlinkLED1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_LED1 */
void LED1(void *argument)
{
  /* USER CODE BEGIN LED1 */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
    osDelay(300);
  }
  /* USER CODE END LED1 */
}

/* USER CODE BEGIN Header_LED2 */
/**
* @brief Function implementing the BlinkLED2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED2 */
void LED2(void *argument)
{
  /* USER CODE BEGIN LED2 */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
    osDelay(500);
  }
  /* USER CODE END LED2 */
}

/* USER CODE BEGIN Header_CDS1 */
/**
* @brief Function implementing the SendCds1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CDS1 */
void CDS1(void *argument)
{
  /* USER CODE BEGIN CDS1 */
	// IN15
	uint32_t ADC_Value_32[1]={0};
	uint16_t ADC_Value_16;

	HAL_ADC_Start_DMA(&hadc1, ADC_Value_32, 1);

  /* Infinite loop */
  for(;;)
  {
	  ADC_Value_16=(uint16_t)ADC_Value_32[0];
	  ADC_Value_16 &= 0xFFF;

	  printf("\n ADC Value=%d", ADC_Value_16);
	  HAL_ADC_Start_DMA(&hadc1, ADC_Value_32, 1);

	  osDelay(1000);

  }
  /* USER CODE END CDS1 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
