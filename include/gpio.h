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

#define POWER_LOCK_Pin LL_GPIO_PIN_11
#define POWER_LOCK_GPIO_Port GPIOA
#define PWR_SW_INT_Pin LL_GPIO_PIN_12
#define PWR_SW_INT_GPIO_Port GPIOA
#define LED_PWR_PB_Pin LL_GPIO_PIN_3
#define LED_PWR_PB_GPIO_Port GPIOB


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

void GpioInit(void);

#ifdef __cplusplus
}
#endif

#endif
/****END OF FILE****/
