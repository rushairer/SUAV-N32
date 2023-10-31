

#include "hw_timer.h"
#include "../motor/motor.h"

static hw_timer_iqr_callback pTickCallback;

void TIM6_IRQHandler(void)
{
    if (TIM_Interrupt_Status_Get(TIM6, TIM_INT_UPDATE) != RESET) {
        TIM_Interrupt_Status_Clear(TIM6, TIM_INT_UPDATE);
        pTickCallback();
    }
}

/**
 * @brief  Configure the TIM IRQ Handler.
 * @param  None
 * @retval None
 *
 * Three sets of 32-bit timer with 24-bit up counter and one 8-bit prescale counter
 *
 */
void hw_tim6_init(hw_timer_iqr_callback cb)
{
    //----------------------------------------------------------------
    pTickCallback = cb;

    //----------------------------------------------------------------

    //----------------------------------------------------------------
    // TIM6 每1ms中断一次。使能中断，作为系统滴答。

    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM6);

    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = 1000;
    TIM_TimeBaseStructure.Prescaler = 64 - 1;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_Base_Initialize(TIM6, &TIM_TimeBaseStructure);

    TIM_Base_Reload_Mode_Set(TIM6, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    TIM_Interrupt_Enable(TIM6, TIM_INT_UPDATE);

    TIM_On(TIM6);
}

// 每1us+1，计时用
void hw_tim4_init(void)
{
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM4);

    //  NVIC_InitType NVIC_InitStructure;
    //  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    //  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    //  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //  NVIC_Initializes(&NVIC_InitStructure);

    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = 65535;
    TIM_TimeBaseStructure.Prescaler = 64 - 1;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_Base_Initialize(TIM4, &TIM_TimeBaseStructure);

    TIM_Base_Reload_Mode_Set(TIM4, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    //  TIM_Interrupt_Enable(TIM4, TIM_INT_UPDATE);

    TIM_On(TIM4);
}

/** 四路电机输出 PA6 PA7 PB0 PB1
 */
void hw_tim3_init(void)
{
    GPIO_InitType GPIO_InitStructure;
    OCInitType TIM_OCInitStructure;

/* TIM3_REMAP0_CH1 */
#define TIM3_REMAP0_CH1_PORT GPIOA
#define TIM3_REMAP0_CH1_PIN  GPIO_PIN_6
#define TIM3_REMAP0_CH1_AF   GPIO_AF3_TIM3
/* TIM3_REMAP0_CH2 */
#define TIM3_REMAP0_CH2_PORT GPIOA
#define TIM3_REMAP0_CH2_PIN  GPIO_PIN_7
#define TIM3_REMAP0_CH2_AF   GPIO_AF3_TIM3
/* TIM3_REMAP0_CH3 */
#define TIM3_REMAP0_CH3_PORT GPIOB
#define TIM3_REMAP0_CH3_PIN  GPIO_PIN_0
#define TIM3_REMAP0_CH3_AF   GPIO_AF3_TIM3
/* TIM3_REMAP0_CH4 */
#define TIM3_REMAP0_CH4_PORT GPIOB
#define TIM3_REMAP0_CH4_PIN  GPIO_PIN_1
#define TIM3_REMAP0_CH4_AF   GPIO_AF3_TIM3

    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    GPIO_Structure_Initialize(&GPIO_InitStructure);

    GPIO_InitStructure.Pin            = TIM3_REMAP0_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current   = GPIO_DS_4MA;
    GPIO_InitStructure.GPIO_Alternate = TIM3_REMAP0_CH1_AF;
    GPIO_Peripheral_Initialize(TIM3_REMAP0_CH1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin            = TIM3_REMAP0_CH2_PIN;
    GPIO_InitStructure.GPIO_Alternate = TIM3_REMAP0_CH2_AF;
    GPIO_Peripheral_Initialize(TIM3_REMAP0_CH2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin            = TIM3_REMAP0_CH3_PIN;
    GPIO_InitStructure.GPIO_Alternate = TIM3_REMAP0_CH3_AF;
    GPIO_Peripheral_Initialize(TIM3_REMAP0_CH3_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin            = TIM3_REMAP0_CH4_PIN;
    GPIO_InitStructure.GPIO_Alternate = TIM3_REMAP0_CH4_AF;
    GPIO_Peripheral_Initialize(TIM3_REMAP0_CH4_PORT, &GPIO_InitStructure);

    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM3);

    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = MOTOR_PWM_PERIOD;
    TIM_TimeBaseStructure.Prescaler = 4 - 1;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_Base_Initialize(TIM3, &TIM_TimeBaseStructure);

    TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);

    /* Channel 1, 2 and 3 Configuration in PWM mode */
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = 0;
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_LOW;
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_LOW;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_SET;
    TIM_OCInitStructure.OcNIdleState = TIM_OCN_IDLE_STATE_RESET;
    TIM_Output_Channel1_Initialize(TIM3, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = 0;
    TIM_Output_Channel2_Initialize(TIM3, &TIM_OCInitStructure);
    TIM_OCInitStructure.Pulse = 0;
    TIM_Output_Channel3_Initialize(TIM3, &TIM_OCInitStructure);
    TIM_OCInitStructure.Pulse = 0;
    TIM_Output_Channel4_Initialize(TIM3, &TIM_OCInitStructure);

    TIM_On(TIM3);
}
