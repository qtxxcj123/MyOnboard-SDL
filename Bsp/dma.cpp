#include "dma.h"
uint8_t sendBuffer[BUFF_SIZE];
void DMA_Config()
{
  DMA_InitTypeDef DMA_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA2ʱ��ʹ�� 
  DMA_DeInit(DMA1_Stream6);
  while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){}//�ȴ�DMA������ 
    /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)sendBuffer;//DMA �洢��0��ַ            ��Ҫ�޸�
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = BUFF_SIZE;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);//��ʼ��DMA Stream  
}

void DMA_Enable(uint8_t len)
{
  DMA_Cmd(DMA1_Stream6, DISABLE);                      //�ر�DMA���� 
	
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA1_Stream6,len);          //���ݴ�����  
 
	DMA_Cmd(DMA1_Stream6, ENABLE);                      //����DMA���� 
}


void DMA_Start(uint8_t len)
{
  USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
  DMA_Enable(len);
  while(1)
  {
		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//�ȴ�DMA2_Steam7�������
		{ 
      DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//���DMA2_Steam7������ɱ�־
			break; 
		}
  }
}

