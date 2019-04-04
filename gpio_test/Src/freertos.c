/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
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
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osMutexId myMutex01Handle;
osSemaphoreId myBinarySem01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Task0(void const * argument);
void Task1(void const * argument);
void Task2(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Create the mutex(es) */
    /* definition and creation of myMutex01 */
    osMutexDef(myMutex01);
    myMutex01Handle = osMutexCreate(osMutex(myMutex01));

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* Create the semaphores(s) */
    /* definition and creation of myBinarySem01 */
    osSemaphoreDef(myBinarySem01);
    myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

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
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, Task0, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* definition and creation of myTask02 */
    osThreadDef(myTask02, Task1, osPriorityIdle, 0, 128);
    myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

    /* definition and creation of myTask03 */
    osThreadDef(myTask03, Task2, osPriorityIdle, 0, 128);
    myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Task0 */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Task0 */
void Task0(void const * argument) {

    /* USER CODE BEGIN Task0 */
    /* Infinite loop */
    for(;;) {
        osDelay(1);
    }
    /* USER CODE END Task0 */
}

/* USER CODE BEGIN Header_Task1 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task1 */
void Task1(void const * argument) {
    /* USER CODE BEGIN Task1 */

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

    /*Configure GPIO pin : PA_4 */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PA_5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    for(;;) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);  // LED
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);  // A3
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  // A4
        HAL_Delay(100);
        osDelay(100);
    }
    /* USER CODE END Task1 */
}

/* USER CODE BEGIN Header_Task2 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task2 */
void Task2(void const * argument) {
    /* USER CODE BEGIN Task2 */
    /* Infinite loop */
    char buffer[16] = {0};
    int cnt = 0;
    for(;;) {
        cnt += 1;
        snprintf(&buffer[0], sizeof(buffer), "%i\n", cnt);
        HAL_UART_Transmit(&huart2, (uint8_t *)&buffer[0], strlen(buffer), 10);
        osDelay(100);
        HAL_Delay(100);
    }
    /* USER CODE END Task2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
