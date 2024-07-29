/**
 *  \file 		gpio.c
 *  \brief		GPIO configuration file
 *  \details	It contain all GPIO init and handling function
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

/* INCLUDES */

#include <gpio.h>

/* VARIABLES */

/* FUNCTION */

/**
 * \brief 	Function for init GPIO peripherals
 * \param 	void
 * \see 	MainLogicInit()
 * \note 	Function should contain all action to execute, before GPIO using.
 * \return 	void
 */
void GpioInit(void){
	  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
	  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

	  /**/
	  LL_GPIO_ResetOutputPin(LED_PWR_PB_GPIO_Port, LED_PWR_PB_Pin);

	  /**/
	  LL_GPIO_SetOutputPin(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin);

	  /**/
	  GPIO_InitStruct.Pin = POWER_LOCK_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  LL_GPIO_Init(POWER_LOCK_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = LED_PWR_PB_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	  LL_GPIO_Init(LED_PWR_PB_GPIO_Port, &GPIO_InitStruct);

	
	  /**/
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE6);

	  /**/
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE7);

	  /**/
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE8);

	  /**/
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE12);

	  /**/
	  LL_GPIO_SetPinPull(PWR_SW_INT_GPIO_Port, PWR_SW_INT_Pin, LL_GPIO_PULL_NO);

	  /**/
	  LL_GPIO_SetPinMode(PWR_SW_INT_GPIO_Port, PWR_SW_INT_Pin, LL_GPIO_MODE_INPUT);

	  /**/
	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_12;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	  LL_EXTI_Init(&EXTI_InitStruct);
}

/****END OF FILE****/
