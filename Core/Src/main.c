/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "led/led_controller.h"
#include "schedule_controller.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_tim.h"
#include <stdint.h>
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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

  
/* USER CODE BEGIN PV */

/* Notes as float frequencies (Hz) */


/* Song note structure - frequency and duration */
typedef struct {
  float frequency;    /* Frequency in Hz (0 = silence/rest) */
  uint16_t duration;  /* Duration in milliseconds */
} NoteEvent_t;



/* Simplified Gravity Falls main theme motif */
static const NoteEvent_t song_gravity_falls[] = {
    {F4, 150},
    
  {D4, 150},
    {A3, 150},
    {D4, 150},

    {F4, 150},
    {D4, 150},
    {A3, 150},
    {D4, 150},

    {F4, 150},
    {C4, 150},
    {A3, 150},
    {C4, 150},

    {F4, 150},
    {C4, 150},
    {A3, 150},
    {C4, 150},

    {E4, 150},
    {D4b, 150},
    {A3, 150},
    {D4b, 150},

    {E4, 150},
    {D4b, 150},
    {A3, 150},
    {D4b, 150},

    {E4, 150},
    {D4b, 150},
    {A3, 150},
    {D4b, 150},

    {E4, 150},
    {D4b, 150},
    {A3, 150},
    {D4b, 150},


    {D4, 600},
    {E4, 300},
    {F4, 600},
    {0, 50},

    {A4, 300},
    {G4, 300},
    {A4, 300},
    {C4, 600},
    {0, 50},

    {D4, 600},
    {E4, 300},
    {F4, 300},
    {E4, 300},

    {G4, 300},
    {A4, 300},
    {G4, 300},
    {F4, 300},
    {0, 50},

    {F4, 150},
    {F4, 150},
    {F4, 150},

    {A4, 150},
    {A4, 150},
    {G4, 150},
    {F4, 150},
    {0, 50},

    {A4, 150},
    {A4, 150},
    {A4, 150},
    {G4, 150},
    {A4, 150},
    {G4, 150},
    {F4, 150},
    {0, 50},

    {F4, 150},
    {F4, 150},
    {F4, 150},
    {A4, 150},
    {A4, 150},
    {G4, 150},
    {F4, 150},
    {0, 100},


    {A4, 150},
    {A4, 150},
    {A4, 150},
    {0, 150},

    {D5b, 150},
    {D5b, 150},
    {D5b, 150},
    {0, 150},

    {F4, 150},
    {F4, 150},
    {F4, 150},
    {A4, 150},
    {A4, 150},
    {G4, 150},

    {F4, 150},
{0, 150},

    {B4b, 150},
    {B4b, 150},
    {B4b, 150},
    {G4, 300},
    {C5, 300},
    {A4, 300},
    {D5b, 300},

    {F4, 150},
    {D4, 150},
    {F4, 150},
    {A4, 150},
    {E4, 150},
    {D4b, 150},
    {A4, 150},
    {D5b, 100},
    {D5, 300},

    {D3, 400}
};

#define GRAVITY_FALLS_LENGTH (sizeof(song_gravity_falls) / sizeof(song_gravity_falls[0]))
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};

      __HAL_RCC_GPIOC_CLK_ENABLE();

      GPIO_InitStruct.Pin = GPIO_PIN_13; // Blue button

      LED_Controller_Init();

      /* initialize the schedule controller */
      ScheduleController_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
      /* USER CODE BEGIN WHILE */
    




  while (1)
  {
      ScheduleController_Process();

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 79;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 10;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


static void play_note_impl(float frequency, uint16_t duration)
{
  (void)duration;
  if (frequency <= 0.0f) {
        /* Rest/silence */
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
    } else {
        /* Calculate timer parameters for desired frequency
         * System clock: 80 MHz
         * Formula: freq = timer_clock / (prescaler * (period + 1))
         * We use period = 1000 for good resolution
         */
        float timer_clock = 80000000.0f;  /* 80 MHz */
        uint32_t period = 1000;
        uint32_t prescaler = (uint32_t)((timer_clock / (frequency * (float)period)) - 1.0f);
        
        /* Limit prescaler to 16-bit */
        if (prescaler > 65535) prescaler = 65535;
        if (prescaler < 1) prescaler = 1;
        
        __HAL_TIM_SET_PRESCALER(&htim2, prescaler);
        __HAL_TIM_SET_AUTORELOAD(&htim2, period);

        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, /* (period / 100)*1 */5);  //1% duty cycle for square wave
        
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    }
}

/**
 * @brief Stop note callback (used by scheduler)
 */
static void stop_note_callback(void)
{
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}
/**
 * @brief Play a song from a note sequence
 * @param song Array of NoteEvent_t structures
 * @param num_notes Number of notes in the song
 */
static const NoteEvent_t *g_song = NULL;
static size_t g_song_length = 0;
static size_t g_song_index = 0;

void Play_Note(float frequency, uint16_t duration)
{
    play_note_impl(frequency, duration);
    
    /* Schedule note stop after duration */
    ScheduleController_Schedule(duration, stop_note_callback);
}

static void song_step_cb(void)
{
    if (g_song == NULL || g_song_index >= g_song_length) {
        return;
    }

    const NoteEvent_t note = g_song[g_song_index++];
    Play_Note(note.frequency, note.duration);

    if (g_song_index < g_song_length) {
        ScheduleController_Schedule(note.duration, song_step_cb);
    }
}

void play_song_internal(const NoteEvent_t *song, size_t num_notes)
{
    g_song = song;
    g_song_length = num_notes;
    g_song_index = 0;

    if (g_song == NULL || g_song_length == 0) {
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        return;
    }

    song_step_cb();
}


void Play_GravityFallsTheme(void)
{
  play_song_internal(song_gravity_falls, GRAVITY_FALLS_LENGTH);
}


uint32_t press_start_time = 0;
uint8_t is_pressed = 0;


volatile uint8_t click_count = 0;
volatile uint8_t click_timer_armed = 0;

static void evaluate_clicks_cb(void)
{
    uint8_t count = click_count;
    click_count = 0;
    click_timer_armed = 0;

    if (count == 1) {
      LED_Controller_Toggle();
      Play_GravityFallsTheme();
    } else if (count == 2) {
        LED_Controller_PrevAnimation();
    } else if (count >= 3) {
        LED_Controller_NextAnimation();
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  
    if (GPIO_Pin == B1_Pin)
    {
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
        {
            press_start_time = HAL_GetTick();
            is_pressed = 1;
        }
        else
        {
            if (is_pressed)
            {
                uint32_t duration = HAL_GetTick() - press_start_time;

                if (duration > 500)
                {
                    LED_Controller_Toggle();
                    (void)ScheduleController_ScheduleFromISR(100, LED_Controller_Toggle);

                    click_count = 0;
                    click_timer_armed = 0;
                }
                else if (duration > 50)
                {
                    click_count++;
                    if (!click_timer_armed) {
                        click_timer_armed = 1;
                        (void)ScheduleController_ScheduleFromISR(300, evaluate_clicks_cb);
                    }
                }
                is_pressed = 0;
            }
        }
    }
}
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
