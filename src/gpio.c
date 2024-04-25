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
	  LL_GPIO_ResetOutputPin(RA_ENABLE_GPIO_Port, RA_ENABLE_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(MCU_OUT_PWR_ENABLE_GPIO_Port, MCU_OUT_PWR_ENABLE_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(LED_CHARGE_GPIO_Port, LED_CHARGE_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(LED_LOWBAT_GPIO_Port, LED_LOWBAT_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(VBUS_DISCH_GPIO_Port, VBUS_DISCH_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(LED_PWR_PB_GPIO_Port, LED_PWR_PB_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(VS_SW_DRV_CH1_GPIO_Port, VS_SW_DRV_CH1_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(VS_SW_DRV_CH2_GPIO_Port, VS_SW_DRV_CH2_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(MCU_PWR_1_GPIO_Port, MCU_PWR_1_Pin);

	  /**/
	  LL_GPIO_SetOutputPin(POWER_LOCK_GPIO_Port, POWER_LOCK_Pin);

	  /**/
	  GPIO_InitStruct.Pin = SBC_PWR_ON_SNS_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(SBC_PWR_ON_SNS_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = STUSB_GPIO_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  LL_GPIO_Init(STUSB_GPIO_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = STUSB_ALERT_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  LL_GPIO_Init(STUSB_ALERT_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = RA_ENABLE_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(RA_ENABLE_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = DIGITAL_BOARD_RST_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  LL_GPIO_Init(DIGITAL_BOARD_RST_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = MCU_OUT_PWR_ENABLE_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(MCU_OUT_PWR_ENABLE_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = CHRG_OK_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	  LL_GPIO_Init(CHRG_OK_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = LED_CHARGE_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(LED_CHARGE_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = LED_LOWBAT_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(LED_LOWBAT_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = VS_SW_CH1_PG_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(VS_SW_CH1_PG_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = VS_SW_CH2_PG_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(VS_SW_CH2_PG_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = VBUS_DISCH_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	  LL_GPIO_Init(VBUS_DISCH_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = PG_5VDC_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(PG_5VDC_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = PG_12VDC_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(PG_12VDC_GPIO_Port, &GPIO_InitStruct);

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
	  GPIO_InitStruct.Pin = VS_SW_DRV_CH1_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	  LL_GPIO_Init(VS_SW_DRV_CH1_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = MCU_PWR_2_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(MCU_PWR_2_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = MCU_PWR_1_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(MCU_PWR_1_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = VS_SW_DRV_CH2_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	  LL_GPIO_Init(VS_SW_DRV_CH2_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE6);

	  /**/
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE7);

	  /**/
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE8);

	  /**/
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE12);

	  /**/
	  LL_GPIO_SetPinPull(IND_ADPT_SNS_GPIO_Port, IND_ADPT_SNS_Pin, LL_GPIO_PULL_NO);

	  /**/
	  LL_GPIO_SetPinPull(USBC_ADPT_SNS_GPIO_Port, USBC_ADPT_SNS_Pin, LL_GPIO_PULL_DOWN);

	  /**/
	  LL_GPIO_SetPinPull(BAT_REMOVAL_GPIO_Port, BAT_REMOVAL_Pin, LL_GPIO_PULL_UP);

	  /**/
	  LL_GPIO_SetPinPull(PWR_SW_INT_GPIO_Port, PWR_SW_INT_Pin, LL_GPIO_PULL_NO);

	  /**/
	  LL_GPIO_SetPinMode(IND_ADPT_SNS_GPIO_Port, IND_ADPT_SNS_Pin, LL_GPIO_MODE_INPUT);

	  /**/
	  LL_GPIO_SetPinMode(USBC_ADPT_SNS_GPIO_Port, USBC_ADPT_SNS_Pin, LL_GPIO_MODE_INPUT);

	  /**/
	  LL_GPIO_SetPinMode(BAT_REMOVAL_GPIO_Port, BAT_REMOVAL_Pin, LL_GPIO_MODE_INPUT);

	  /**/
	  LL_GPIO_SetPinMode(PWR_SW_INT_GPIO_Port, PWR_SW_INT_Pin, LL_GPIO_MODE_INPUT);

	  /**/
	  LL_GPIO_SetOutputPin(DIGITAL_BOARD_RST_GPIO_Port, DIGITAL_BOARD_RST_Pin);

	  /**/
	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_6;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
	  LL_EXTI_Init(&EXTI_InitStruct);

	  /**/
	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_7;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
	  LL_EXTI_Init(&EXTI_InitStruct);

	  /**/
	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_8;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
	  LL_EXTI_Init(&EXTI_InitStruct);

	  /**/
	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_12;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	  LL_EXTI_Init(&EXTI_InitStruct);
}

/****END OF FILE****/
