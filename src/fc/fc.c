

#include "fc.h"
#include "decode.h"
#include "pid.h"
#include "../ahrs/accgyro.h"
#include "../ahrs/imu.h"
#include "../ahrs/altitude.h"
#include "../schedule/ticks.h"
#include "../motor/motor.h"
#include "../common/mlib.h"

fc_ctrl_s fc;

void flight_ctrl(void)
{    
  static uint16_t previousIMUUpdateTime;
  uint16_t currentTime;
  float deltaT;

  currentTime = micros16();
  deltaT = (uint16_t)(currentTime-previousIMUUpdateTime) * 1e-6;
  if(deltaT >= 0.001f) {
    previousIMUUpdateTime = currentTime;   
          
    accgyro_update();
    imuCalculateEstimatedAttitude();
    pid(deltaT);
    motor_pwm_output();
  }  
}

void fc_init(void)
{
  fc.flags.uartCmdNew = 0;
  fc.flags.motorArmed = 0;
  fc.flags.motorIdle = 0;
  fc.flags.bBatteryLow = 0;
  
  pidconf.angleTrim[0] = 0;
  pidconf.angleTrim[1] = 0;
  pidconf.angleTrim[2] = 0;
}
