/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu16 ADC_DMA_Buf[ADC_Sample][ADC_Channel] = {0};
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : ADC_Average
**功能 : 將 ADC 轉換後的資料取平均
**輸入 : ADC_AveTr
**輸出 : None
**使用 : ADC_Average(ADC_AveTr);
**=====================================================================================================*/
/*=====================================================================================================*/
void ADC_Config( void )
{
  DMA_InitTypeDef DMA_InitStruct;
  ADC_InitTypeDef ADC_InitStruct;
  ADC_CommonInitTypeDef ADC_CommonInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* ADC Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* ADC_I PA4 */  /* ADC_V PA5 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ADC DMA Init *************************************************************/
  DMA_InitStruct.DMA_Channel = DMA_Channel_0;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)ADC1_DR_ADDRESS;               // Peripheral address
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)&ADC_DMA_Buf;                     // Memory address
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;                        // Peripheral to Memory
  DMA_InitStruct.DMA_BufferSize = ADC_Sample*ADC_Channel;                     // Memory Buffer Size
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;               // Peripheral address 遞增 Disable
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                        // Memory address 遞增 Enable
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    // Peripheral Data Size 16bit
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;            // Memory Data Size 16bit
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                                // 循環模式 Enable
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;                            // ADC DMA通道 高優先級
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;                         // DMA FIFO Disable
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStruct);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                       // 獨立模式
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;                    // 預分頻2
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;        // ADC DMA Mode Disable
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;   // 轉換延遲時間
  ADC_CommonInit(&ADC_CommonInitStruct);

  /* ADC Init *****************************************************************/
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                         // 解析度 12bit
  ADC_InitStruct.ADC_ScanConvMode = ENABLE;                                   // 掃描模式
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                             // 連續轉換模式
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;    // 外部觸發 Disable
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                         // ADC數據右對齊
  ADC_InitStruct.ADC_NbrOfConversion = ADC_Channel;                           // 轉換ADC通道數目
  ADC_Init(ADC1, &ADC_InitStruct);

  /* ADC Regular Config *******************************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_3Cycles);  	// ADC_I
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_3Cycles);  	// ADC_V

  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_ContinuousModeCmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_SoftwareStartConv(ADC1);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : ADC_Average
**功能 : 將 ADC 轉換後的資料取平均
**輸入 : ADC_AveTr
**輸出 : None
**使用 : ADC_Average(ADC_AveTr);
**=====================================================================================================*/
/*=====================================================================================================*/
void ADC_Average( u16* pADC_AveTr )
{
  u8 i, j;
  u32 ADC_DMA_Tmp[ADC_Channel] = {0};

  for(i=0; i<ADC_Channel; i++) {
    for(j=0; j<ADC_Sample; j++)
      ADC_DMA_Tmp[i] += ADC_DMA_Buf[j][i];
    pADC_AveTr[i] = (u16)(ADC_DMA_Tmp[i] / ADC_Sample);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
