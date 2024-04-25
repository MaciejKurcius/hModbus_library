/**
 * \file		bsp.c
 * \brief		Boar Support Package file
 * \details		It contain all peripherals init function (if they don't have another self file)
 * \author		Maciej Kurcius
 * \version		1.0
 * \date		24.11.2021
 */

/* INCLUDES */

#include <bsp.h>

/* FUNCTION */

/**
 * \brief 	Function for initialize SysTick timer.
 * \param 	void
 * \see 	MainLogicInit()
 * \note 	This function should be call, before processor enters to main loop program.
 * \return 	void
 */
void SysTickInit(void){
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
}

/**
 * \brief 	Function for initialize System Clock.
 * \param 	void
 * \see 	MainLogicInit()
 * \note 	This function should be call, before processor enters to main loop program.
 * \return 	void
 */
void SystemClockConfig(void){
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0){
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_MSI_Enable();

   /* Wait till MSI is ready */
  while(LL_RCC_MSI_IsReady() != 1){
  }
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_5);
  LL_RCC_MSI_SetCalibTrimming(0);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI){
  }
  LL_Init1msTick(2097000);
  LL_SetSystemCoreClock(2097000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);
}

/**
 * \brief 	Function for prepare board hardware to work
 * \param 	void
 * \see 	MainLogicInit()
 * \see 	SysTickInit()
 * \see 	SystemClockConfig()
 * \see 	GpioInit()
 * \see 	AdcInit()
 * \see 	I2cInit()
 * \see 	UartInit()
 * \note 	Function contain all action execute only one, after power is on.
 * \return 	void
 */
void BoardInit(void){
	SysTickInit();
	SystemClockConfig();
	GpioInit();
	AdcInit();
	I2cInit();
	UartInit();
}

/****END OF FILE****/
