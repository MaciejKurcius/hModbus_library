#ifndef MAIN_H
#define MAIN_H


/* Includes ------------------------------------------------------------------*/
#include <stm32l0xx_ll_adc.h>
#include <stm32l0xx_ll_dma.h>
#include <stm32l0xx.h>
#include <stm32l0xx_ll_i2c.h>
#include <stm32l0xx_ll_crs.h>
#include <stm32l0xx_ll_rcc.h>
#include <stm32l0xx_ll_bus.h>
#include <stm32l0xx_ll_system.h>
#include <stm32l0xx_ll_exti.h>
#include <stm32l0xx_ll_cortex.h>
#include <stm32l0xx_ll_utils.h>
#include <stm32l0xx_ll_pwr.h>
#include <stm32l0xx_ll_tim.h>
#include <stm32l0xx_ll_usart.h>
#include <stm32l0xx_ll_gpio.h>
#include <eeprom.h>

#if defined(USE_FULL_ASSERT)
    #include "stm32_assert.h"
#endif

// Rest includes
#include <bsp.h>
#include <logic.h>
#include <config.h>
#include <bq25713.h>
#include <Uart_Lib.h>
#include <adc.h>

/* FUNCTION PROTOTYPES */

void Error_Handler(void);
// int main(void);

#endif /* MAIN_H */