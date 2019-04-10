#include "dma.h"
uint8_t sendBuffer[BUFF_SIZE];
void DMA_Config()
{
  DMA_InitTypeDef DMA_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA2时钟使能 
  DMA_DeInit(DMA1_Stream6);
  while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){}//等待DMA可配置 
    /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)sendBuffer;//DMA 存储器0地址            需要修改
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = BUFF_SIZE;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);//初始化DMA Stream  
}

void DMA_Enable(uint8_t len)
{
  DMA_Cmd(DMA1_Stream6, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA1_Stream6,len);          //数据传输量  
 
	DMA_Cmd(DMA1_Stream6, ENABLE);                      //开启DMA传输 
}


void DMA_Start(uint8_t len)
{
  USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
  DMA_Enable(len);
  while(1)
  {
		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//等待DMA2_Steam7传输完成
		{ 
      DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//清除DMA2_Steam7传输完成标志
			break; 
		}
  }
}

