#include "hModbusLib_conf.h"
#include <stm32l0xx_ll_utils.h>

extern volatile uint32_t SysTick_counter;

void hModbusDelay(uint32_t DelayTime){
    LL_mDelay(DelayTime);
}

uint32_t hModbusGetSystemClockUs(void){
    return SysTick_counter;
}

void hModbusUsartTx8(uint8_t TxData){

}

uint8_t hModbusUsartRx8(void){
    return 0;
}

