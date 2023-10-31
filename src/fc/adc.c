
#include "adc.h"
#include "../hardware/hw_adc.h"
#include "fc.h"
#include <stdio.h>

uint8_t adcCtrlTimer;
float batValue;

void adc_init(void)
{
    hw_adc_init();
}

void adc_update(void)
{
    static uint32_t _adcSum = 0;
    static uint8_t _adcCnt  = 0;

    if (adcCtrlTimer >= 1) {
        adcCtrlTimer = 0;

        _adcSum += ADCConvertedValue;
        if (++_adcCnt >= 64) { // 32*20 = 640ms
            batValue = _adcSum >> 6;
            _adcCnt  = 0;
            _adcSum  = 0;

            // 3.3v 稳压，2:1分压
            // adc / 4096 * 3.3 * 3 / 1
            batValue = batValue * 3 * 3.3f / 1.0f / 4096;

            if (batValue <= 2.80f) {
                fc.flags.bBatteryLow = 1;
            }

            //      printf("电池电压: %.2fv\r\n", batValue);
        }
    }
}
