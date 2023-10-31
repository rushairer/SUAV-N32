

#pragma once     

//------------------------------------------------------------------------------------
#include "../platform.h" 


//------------------------------------------------------------------------------------
#define SOFT_I2C_SCL_PIN                         GPIO_PIN_6
#define SOFT_I2C_SCL_GPIO_PORT                   GPIOB
#define SOFT_I2C_SCL_GPIO_CLK                    RCC_AHB_PERIPH_GPIOB

#define SOFT_I2C_SCL_HIGH                        {SOFT_I2C_SCL_GPIO_PORT->PBSC = SOFT_I2C_SCL_PIN;}
#define SOFT_I2C_SCL_LOW                         {SOFT_I2C_SCL_GPIO_PORT->PBC = SOFT_I2C_SCL_PIN;}

#define SOFT_I2C_SDA_PIN                         GPIO_PIN_7
#define SOFT_I2C_SDA_GPIO_PORT                   GPIOB
#define SOFT_I2C_SDA_GPIO_CLK                    RCC_AHB_PERIPH_GPIOB

#define SOFT_I2C_SDA_HIGH                        {SOFT_I2C_SDA_GPIO_PORT->PBSC = SOFT_I2C_SDA_PIN;}
#define SOFT_I2C_SDA_LOW                         {SOFT_I2C_SDA_GPIO_PORT->PBC = SOFT_I2C_SDA_PIN;}
#define SOFT_I2C_SDA_READ                        (SOFT_I2C_SDA_GPIO_PORT->PID & SOFT_I2C_SDA_PIN)


//------------------------------------------------------------------------------------
void hw_soft_i2c_init(void);

void hw_soft_i2c_sda_output(void);
void hw_soft_i2c_sda_input(void);



