/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
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
TX_THREAD              MutexThreadOne;
TX_THREAD              MutexThreadTwo;
TX_MUTEX               Mutex_0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void    MutexThreadOne_Entry(ULONG thread_input);
void    MutexThreadTwo_Entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer;

   /* Allocate the stack for MutexThreadOne.  */
   if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                        APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
   {
     ret = TX_POOL_ERROR;
   }

   /* Create MutexThreadOne.  */
   if (tx_thread_create(&MutexThreadOne, "Mutex Thread One",
                   MutexThreadOne_Entry, 0, pointer, APP_STACK_SIZE,
             MUTEX_THREAD_ONE_PRIO, MUTEX_THREAD_ONE_PREEMPTION_THRESHOLD,
                        TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
   {
     ret = TX_THREAD_ERROR;
   }

   /* Allocate the stack for MutexThreadTwo.  */
   if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                        APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
   {
     ret = TX_POOL_ERROR;
   }

   /* Create MutexThreadTwo.  */
   if (tx_thread_create(&MutexThreadTwo, "Mutex Thread Two",
                   MutexThreadTwo_Entry, 0, pointer, APP_STACK_SIZE,
             MUTEX_THREAD_TWO_PRIO, MUTEX_THREAD_TWO_PREEMPTION_THRESHOLD,
                        TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
   {
     ret = TX_THREAD_ERROR;
   }

   if (tx_mutex_create(&Mutex_0, "mutex 0", TX_NO_INHERIT))
   {
     ret = TX_THREAD_ERROR;
   }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing the MutexThreadOne thread.
  * @param  thread_input: Not used
  * @retval None
  */
void MutexThreadOne_Entry(ULONG thread_input)
{
  UINT status;
  (void) thread_input;
  /* Infinite loop */
  while(1)
  {
    /* Get the mutex with suspension */
    status = tx_mutex_get(&Mutex_0, TX_WAIT_FOREVER);

    /* Check status */
    if (status != TX_SUCCESS)
      break;

    /* Sleep for 200 ticks to hold the mutex */
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    tx_thread_sleep(200);

    /* Release the mutex. */
    status = tx_mutex_put(&Mutex_0);

    /* Check status. */
    if (status != TX_SUCCESS)
      break;
  }
}

/**
  * @brief  Function implementing the MutexThreadTwo thread.
  * @param  thread_input: Not used
  * @retval None
  */
void MutexThreadTwo_Entry(ULONG thread_input)
{
  UINT status;
  (void) thread_input;
  /* Infinite loop */
  while(1)
  {
    /* Get the mutex with suspension */
    status = tx_mutex_get(&Mutex_0, TX_WAIT_FOREVER);

    /* Check status */
    if (status != TX_SUCCESS)
      break;

    /* Sleep for 200 ticks to hold the mutex */
    tx_thread_sleep(200);

    /* Release the mutex. */
    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
    status = tx_mutex_put(&Mutex_0);

    /* Check status. */
    if (status != TX_SUCCESS)
      break;
  }
}
/* USER CODE END 1 */
