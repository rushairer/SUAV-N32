

#include "hw_adc.h"

__IO uint16_t ADCConvertedValue;

void hw_adc_init(void)
{
  //---------------------------------------------------------------------------------------------
  /* Enable DMA GPIO ADC clocks */
  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA|RCC_AHB_PERIPH_GPIOA|RCC_AHB_PERIPH_ADC);

  /* RCC_ADCHCLK_DIV16*/
  ADC_Clock_Mode_Config(ADC_CKMOD_AHB, RCC_ADCHCLK_DIV16);
  RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8);  

  //---------------------------------------------------------------------------------------------  
  GPIO_InitType GPIO_InitStructure;

  GPIO_Structure_Initialize(&GPIO_InitStructure);
  /* Configure adc input as analog input -------------------------*/
  GPIO_InitStructure.Pin       = GPIO_PIN_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
  GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
      
  //---------------------------------------------------------------------------------------------  
  /* DMA channel1 configuration ----------------------------------------------*/
  ADC_InitType ADC_InitStructure;
  DMA_InitType DMA_InitStructure;

  DMA_Reset(DMA_CH1);
  DMA_InitStructure.PeriphAddr     = (uint32_t)&ADC->DAT;
  DMA_InitStructure.MemAddr        = (uint32_t)&ADCConvertedValue;
  DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
  DMA_InitStructure.BufSize        = 1;
  DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
  DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
  DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_HALFWORD;
  DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_HALFWORD;
  DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_ENABLE;
  DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;
  DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
  DMA_Initializes(DMA_CH1, &DMA_InitStructure);
  DMA_Channel_Request_Remap(DMA_CH1, DMA_REMAP_ADC);
  /* Enable DMA channel1 */
  DMA_Channel_Enable(DMA_CH1);

  ADC_Initializes_Structure(&ADC_InitStructure);
  ADC_InitStructure.MultiChEn      = ENABLE;
  ADC_InitStructure.ContinueConvEn = ENABLE;
  ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
  ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH;
  ADC_InitStructure.ChsNumber      = ADC_REGULAR_LEN_1;
  ADC_Initializes(&ADC_InitStructure);

  /* ADC channel sampletime configuration */
  ADC_Channel_Sample_Time_Config(ADC_Channel_06_PA5, ADC_SAMP_TIME_55CYCLES5);
  /* ADC regular channel configuration */
  ADC_Regular_Sequence_Conversion_Number_Config(ADC_Channel_06_PA5, ADC_REGULAR_NUMBER_1);

  /* Enable ADC DMA */
  ADC_DMA_Transfer_Enable();

  /* Enable ADC */
  ADC_ON();

  /* Check ADC Ready */
  while(ADC_Flag_Status_Get(ADC_RD_FLAG, ADC_FLAG_JENDCA, ADC_FLAG_RDY) == RESET)
      ;

  /* Start ADC1 calibration */
  ADC_Calibration_Operation(ADC_CALIBRATION_ENABLE);
  /* Check the end of ADC1 calibration */
  while (ADC_Calibration_Operation(ADC_CALIBRATION_STS))
      ;
  /* Start ADC Software Conversion */
  ADC_Regular_Channels_Software_Conversion_Operation(ADC_EXTRTRIG_SWSTRRCH_ENABLE);

}

