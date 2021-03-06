/**
  Largely stolen from the STM32cubeL4 tree
  
  STM32Cube_FW_L4_V1.2.0/Projects/STM32L476G-Discovery/Examples/GPIO/GPIO_IOToggle/Src/
  
  This example blinks LED4 and LED5
  (Simplest test... no use of the DSP libraries or any periferals)
  
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"

static GPIO_InitTypeDef  GPIO_InitStruct;
static void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    
    LED4_GPIO_CLK_ENABLE();
    LED5_GPIO_CLK_ENABLE();
    

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin = LED4_PIN;
    HAL_GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED5_PIN;
    HAL_GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStruct);

    while (1) {
        HAL_GPIO_TogglePin(LED4_GPIO_PORT, LED4_PIN);
        HAL_Delay(100);
        HAL_GPIO_TogglePin(LED5_GPIO_PORT, LED5_PIN);
        HAL_Delay(100);
  }
}


/**
  * @brief  System Clock Configuration
  * 
  * Hummels Config to give 80 MHz SYSCLK, with PLL48M1CLK=48 MHz
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000 (MSI Range=6) 
  *            PLL_M                          = 1
  *            PLL_N                          = 60
  *            PLL_R                          = 3 ( SYSCLK = (4MHz)(60/3) = 80 MHz )
  *            PLL_P                          = 7 (No reason for this...)
  *            PLL_Q                          = 5 ( PLL48M1CLK = (4MHz)(60/5) = 48 MHz )
  *            Flash Latency(WS)              = 4
  * 
  * HSE: Not assumed active (xtal not populated on STM32L476G-Discovery)
  * MSI: 4 MHz (multiple speed internal RC Oscillator, configured using Range=6)
  * HSI: 16 MHz (Fixed frequency high speed internal Oscillator)
  * LSI: 32.768 kHz (Low freq internal oscillator)
  * SYSCLK: 80 MHz
  * HCLK (AHB Bus, Core, Memory, and DMA): 80 MHz (Max value)
  * PCLK1 (APB1 Periferals, some USARTs and Timers): 80 MHz (Max value)
  * PCLK2 (APB2 Periferals, some USARTs and Timers): 80 MHz (Max value)
  * 
  * (ALTERNATIVE???  Use the HSI as the reference oscillator.  Change N to 15.)
  * 
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLR = 3;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


void SysTick_Handler(void)
{
  HAL_IncTick();
}

