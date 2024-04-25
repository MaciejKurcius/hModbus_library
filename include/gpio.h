/**
 *  \file 		gpio.h
 *  \brief		GPIO configuration file
 *  \details	It contain all GPIO init and handling function
 *  \author		Maciej Kurcius
 *  \version	1.0
 *  \date		24.11.2021
 */

#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* INCLUDES */

#include <main.h>

/* DEFINES */

#define SBC_PWR_ON_SNS_Pin LL_GPIO_PIN_13
#define SBC_PWR_ON_SNS_GPIO_Port GPIOC
#define TEMP_SENSOR_Pin LL_GPIO_PIN_1
#define TEMP_SENSOR_GPIO_Port GPIOA
#define STUSB_GPIO_Pin LL_GPIO_PIN_2
#define STUSB_GPIO_GPIO_Port GPIOA
#define STUSB_ALERT_Pin LL_GPIO_PIN_3
#define STUSB_ALERT_GPIO_Port GPIOA
#define RA_ENABLE_Pin LL_GPIO_PIN_4
#define RA_ENABLE_GPIO_Port GPIOA
#define DIGITAL_BOARD_RST_Pin LL_GPIO_PIN_5
#define DIGITAL_BOARD_RST_GPIO_Port GPIOA
#define IND_ADPT_SNS_Pin LL_GPIO_PIN_6
#define IND_ADPT_SNS_GPIO_Port GPIOA
#define USBC_ADPT_SNS_Pin LL_GPIO_PIN_7
#define USBC_ADPT_SNS_GPIO_Port GPIOA
#define MCU_OUT_PWR_ENABLE_Pin LL_GPIO_PIN_0
#define MCU_OUT_PWR_ENABLE_GPIO_Port GPIOB
#define CHRG_OK_Pin LL_GPIO_PIN_1
#define CHRG_OK_GPIO_Port GPIOB
#define LED_CHARGE_Pin LL_GPIO_PIN_2
#define LED_CHARGE_GPIO_Port GPIOB
#define LED_LOWBAT_Pin LL_GPIO_PIN_12
#define LED_LOWBAT_GPIO_Port GPIOB
#define VS_SW_CH1_PG_Pin LL_GPIO_PIN_13
#define VS_SW_CH1_PG_GPIO_Port GPIOB
#define VS_SW_CH2_PG_Pin LL_GPIO_PIN_14
#define VS_SW_CH2_PG_GPIO_Port GPIOB
#define VBUS_DISCH_Pin LL_GPIO_PIN_15
#define VBUS_DISCH_GPIO_Port GPIOB
#define BAT_REMOVAL_Pin LL_GPIO_PIN_8
#define BAT_REMOVAL_GPIO_Port GPIOA
#define PG_5VDC_Pin LL_GPIO_PIN_9
#define PG_5VDC_GPIO_Port GPIOA
#define PG_12VDC_Pin LL_GPIO_PIN_10
#define PG_12VDC_GPIO_Port GPIOA
#define POWER_LOCK_Pin LL_GPIO_PIN_11
#define POWER_LOCK_GPIO_Port GPIOA
#define PWR_SW_INT_Pin LL_GPIO_PIN_12
#define PWR_SW_INT_GPIO_Port GPIOA
#define LED_PWR_PB_Pin LL_GPIO_PIN_3
#define LED_PWR_PB_GPIO_Port GPIOB
#define VS_SW_DRV_CH1_Pin LL_GPIO_PIN_4
#define VS_SW_DRV_CH1_GPIO_Port GPIOB
#define MCU_PWR_1_Pin LL_GPIO_PIN_5
#define MCU_PWR_1_GPIO_Port GPIOB
#define MCU_PWR_2_Pin LL_GPIO_PIN_8
#define MCU_PWR_2_GPIO_Port GPIOB
#define VS_SW_DRV_CH2_Pin LL_GPIO_PIN_9
#define VS_SW_DRV_CH2_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* TYPE DEF ENUM */

/* STRUCTURES */

/* VARIABLES */

/* FUNCTION PROTOTYPES */

void GpioInit(void);

#ifdef __cplusplus
}
#endif

#endif
/****END OF FILE****/
