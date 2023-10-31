

#include "platform.h"
#include <stdio.h>
#include "led/led.h"
#include "schedule/ticks.h"
#include "bus/uart.h"
#include "iap/iap.h"
#include "hardware/hw_uart.h"
#include "msp/msp.h"
#include "common/mlib.h"

#include "bus/i2c_soft.h"
#include "ahrs/mpu6050.h"
#include "ahrs/accgyro.h"
#include "ahrs/imu.h"
#include "ahrs/barometer.h"
#include "ahrs/altitude.h"
#include "ahrs/opflow.h"

#include "motor/motor.h"
#include "fc/decode.h"
#include "fc/fc.h"
#include "fc/pid.h"
#include "fc/adc.h"

uint8_t printTestTimer;
uint8_t powerupTimer;

int main(void)
{
    SCB->VTOR = FLASH_BASE | 0x4000;
    iap_for_app_init();

    led_init();
    ticks_init();
    debug_uart_init();
    soft_i2c_init();
    motor_init();
    adc_init();

    printf("suav system start!\r\n");
    delay_ms(200);
    accgyro_init();
    imu_init();
    pid_init();
    baro_init();
    altitude_init();
    opflow_init();

    fc.flags.bAccGyroCalibrationEnb = 1;
    powerupTimer                    = 0;

    printf("suav system ready!\r\n");

    while (1) {
        if (accgyro.gyroCalibrating || accgyro.accCalibrating)
            led_ctrl(LED_GYRO_CALIBRATION);
        else if (fc.flags.bBatteryLow)
            led_ctrl(LED_BATTERY_LOW);
        else if (fc.flags.motorArmed)
            led_ctrl(LED_FLIGHT);
        else
            led_ctrl(LED_IDLE);

        msp();
        annexCode();
        adc_update();

        altitude_ctrl();
        opticalflow_ctrl();

        flight_ctrl();

        //--------------------------------------------------------
        if ((abs(angle[0]) >= 8000) || (abs(angle[1] >= 8000))) {
            fc.flags.motorArmed = 0;
            fc.flags.motorIdle  = 0;
        }

        //--------------------------------------------------------
        if (printTestTimer >= 2) {
            printTestTimer = 0;

            // 姿态角和气压计高度
            printf("%f,%f,%f,%ld,", angle[ROLL] / 100.0f, angle[PITCH] / 100.0f, angle[YAW] / 100.0f, BaroAlt);
            printf("%d,%d,%d,%d\n", rcCommand[ROLL], rcCommand[PITCH], rcCommand[THROTTLE], rcCommand[YAW]);
            //      printf("%.2f\n", batValue);
        }
    }
}
