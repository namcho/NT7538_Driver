/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "NT7538_Driver.h"
#include "LCDInterface.h"

static void LedInits(void);
static void SystemClock_Config(void);

LCD_HW_Interface_t lcdhwinterface_stm32;

int main(void)
{
	SystemClock_Config();
	AssignPointerFunc(&lcdhwinterface_stm32);
	LCDInit(&lcdhwinterface_stm32);
	lcdhwinterface_stm32.pfWriteData(0xFF);

	LedInits();
	for(;;){
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		HAL_Delay(1000);
		lcdhwinterface_stm32.pfWriteData(0xAA);
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);
		HAL_Delay(500);
		lcdhwinterface_stm32.pfWriteData(0x55);
	}
}

void LedInits(){
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_GPIOE_CLK_ENABLE();

	gpio.Mode		= GPIO_MODE_OUTPUT_PP;
	gpio.Pin		= GPIO_PIN_8 | GPIO_PIN_9;
	HAL_GPIO_Init(GPIOE, &gpio);
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

