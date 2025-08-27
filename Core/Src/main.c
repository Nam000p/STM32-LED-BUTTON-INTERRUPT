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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led_control.h"
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

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */
uint8_t count = 0;
CLICK_STATE click_state;
_BOOL increment = FALSE;

uint32_t press_tick = 0;
uint8_t click_count = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */
/**
 * @brief  Set the LED output state.
 *
 * This function controls three LEDs based on the given state value.
 * Each state corresponds to a specific LED configuration.
 *
 * @param[in]  state  Desired LED state:
 *                    - 0: LED1 ON, LED2 OFF, LED3 OFF
 *                    - 1: LED2 ON, LED1 OFF, LED3 OFF
 *                    - 2: LED3 ON, LED1 OFF, LED2 OFF
 *                    - 3: All LEDs OFF
 *
 * @note  This function directly calls LEDx_H()/LEDx_L() macros,
 *        which must be defined to control the GPIO pins.
 *
 * @retval None
 */
void set_led_state(uint8_t state);
/**
 * @brief  Handle button actions based on click state.
 *
 * Updates the global variable "count" depending on the button action:
 *  - SINGLE_CLICK → reset LEDs (count = 0)
 *  - DOUBLE_CLICK → cycle through LEDs (count = 0 → 1 → 2 → 0),
 *                   but only if not in HOLD mode
 *  - HOLD         → turn off all LEDs (special value count = 100)
 *
 * The "increment" flag ensures that DOUBLE_CLICK increments only once
 * per valid event.
 */
void detect_button_state(CLICK_STATE click_state);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

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
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Process the button input and update "count" based on click state
	  detect_button_state(click_state);

	  // Set the LED behavior according to the current value of "count"
	  set_led_state(count);
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

  /*AXI clock gating */
  RCC->CKGAENR = 0xE003FFFF;

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 35;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 27999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 30000;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 55999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void set_led_state(uint8_t state)
{
    switch(state)
    {
        case 0: // LED1 on
        	turn_on_led1();
        	turn_off_led2();
        	turn_off_led3();
            break;
        case 1: // LED2 on
            turn_off_led1();
            turn_on_led2();
            turn_off_led3();
            break;
        case 2: // LED3 on
        	turn_off_led1();
        	turn_off_led2();
        	turn_on_led3();
            break;
        default: // All LED off
        	turn_off_led1();
        	turn_off_led2();
        	turn_off_led3();
        	break;
    }
}
void detect_button_state(CLICK_STATE click_state)
{
    switch(click_state)
    {
        case SINGLE_CLICK:
        	// Reset to base state (count = 0)
        	// ONLY if the previous state was HOLD (100).
        	// This prevents accidental reset when already cycling LEDs.
        	if (count == 100)
        		count = 0;
            break;
        case DOUBLE_CLICK:
            // On double click: increase count cyclically (0 → 1 → 2 → 0)
            // Only increment once per event when "increment" flag is TRUE
            if (increment)
            {
                // If all LEDs are OFF because of HOLD, ignore this event
                // (must be followed by SINGLE_CLICK or ignored completely)
                if (count == 100)
                    break;
                else
                    count = (count + 1) % 3;

                increment = FALSE; // Reset flag to avoid repeated increments
            }
            break;
        case HOLD:
            // If button is held: assign a special value (100 → all LEDs OFF)
            count = 100;
            break;
        default:
            // Do nothing for undefined states
            break;
    }
}
/**
 * @brief  External interrupt callback for button press (B1).
 *
 * This function is triggered whenever the button pin (B1_BUTTON_PIN)
 * changes state (pressed or released).
 *
 * Logic:
 *  - On button press (rising edge): count the clicks and start Timer6
 *    if it's the first click.
 *  - On button release (falling edge): check how long the button was held
 *    using Timer6 counter. If it was a short press (< 2000 ticks) and
 *    at least one click was detected, start Timer7 to evaluate
 *    single/double click.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == B1_BUTTON_PIN)   // Check if interrupt comes from button B1
    {
        // ----- Button Pressed (Pin is HIGH) -----
        if (HAL_GPIO_ReadPin(B1_BUTTON_GPIO_PORT, B1_BUTTON_PIN) == GPIO_PIN_SET)
        {
            click_count++;   // Increase number of clicks detected
            // Start Timer6 on the first click (used to measure press duration)
            if (click_count < 2)
                HAL_TIM_Base_Start_IT(&htim6);
        }
        // ----- Button Released (Pin is LOW) -----
        else
        {
            // Read how long the button was pressed using Timer6 counter
            press_tick = __HAL_TIM_GET_COUNTER(&htim6);

            // If it was a short press (< 2000 ticks) and at least 1 click happened
            // then start Timer7 to check if this is single or double click
            if ((press_tick < 2000) && (click_count >= 1))
            {
                HAL_TIM_Base_Start_IT(&htim7);
            }
            // Reset Timer6 for the next measurement
            __HAL_TIM_SET_COUNTER(&htim6, 0);
            HAL_TIM_Base_Stop_IT(&htim6);
        }
    }
}
/**
 * @brief  Timer interrupt callback.
 *
 * This function is called when a timer configured in interrupt mode expires.
 * It is used together with EXTI (button interrupt) to detect:
 *  - HOLD (long press, via TIM6)
 *  - SINGLE or DOUBLE CLICK (via TIM7)
 *
 * @param  htim: pointer to the timer handle that triggered the interrupt
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // ----- Timer6: Long press (HOLD detection) -----
    if (htim->Instance == TIM6)
    {
        click_count = 0;       // Reset click counter
        click_state = HOLD;    // Button is considered "HOLD"
    }
    // ----- Timer7: Single / Double click detection -----
    if (htim->Instance == TIM7)
    {
        // If the button is still pressed, stop Timer7 (ignore event)
        if (HAL_GPIO_ReadPin(B1_BUTTON_GPIO_PORT, B1_BUTTON_PIN) == GPIO_PIN_SET)
        {
            __HAL_TIM_SET_COUNTER(&htim7, 0);
            HAL_TIM_Base_Stop_IT(&htim7);
        }
        // If two clicks happened before timeout → DOUBLE CLICK
        if (click_count == 2)
        {
            click_state = DOUBLE_CLICK;
            increment = TRUE;   // Allow increment in detect_button_state()
        }
        // Otherwise → SINGLE CLICK
        else
        {
            click_state = SINGLE_CLICK;
        }

        click_count = 0;                  // Reset click counter
        HAL_TIM_Base_Stop_IT(&htim7);     // Stop Timer7 after classification
    }
}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

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
