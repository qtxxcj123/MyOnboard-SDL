/*! @file BspUsart.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Usart helper functions and ISR for board STM32F4Discovery
 *   usart�������ܺ�isr���ڰ��stm 32f4����
 *  @Copyright (c) 2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "stm32f4xx.h"
#include "BspUsart.h"
#include "timer.h"
#include "led.h"

extern int Rx_Handle_Flag;

using namespace DJI::OSDK;

extern Vehicle  vehicle;
extern Vehicle *v;
extern Control  control;

extern bool           isFrame;
bool                  isACKProcessed    = false;
bool                  ackReceivedByUser = false;
extern RecvContainer  receivedFramie;
extern RecvContainer *rFrame;

//��ʼ��IO ����1 
//bound:������
void USART1_Init(unsigned int bound)
{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
}


void USART2_Gpio_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // tx
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // rx
}

void USART3_Gpio_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); // tx
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); // rx
}

/*
 * USART2 is used for receiving commands from PC and
 * printing debug information to PC
 */
void USART2_Config(void)
{
    USART2_Gpio_Config();

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate   = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;
    USART_InitStructure.USART_Parity     = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
    USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART2, ENABLE);

    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
}

/*
 * USART3 is used for communicating with the DJI flight controller USART3������DJI���п�����ͨ��
 * The Baud rate needs to match the Baud rate used by the flight controller ��������Ҫ����п�����ʹ�õĲ�������ƥ��
 */
void USART3_Config(void)
{
    USART3_Gpio_Config();

    USART_InitTypeDef USART_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    USART_InitStructure.USART_BaudRate   = 230400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;
    USART_InitStructure.USART_Parity     = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART3, ENABLE);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET);
}

void USARTxNVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStructure_USART3;
    NVIC_InitStructure_USART3.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure_USART3.NVIC_IRQChannelSubPriority        = 0x00;
    NVIC_InitStructure_USART3.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure_USART3.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure_USART3);

    NVIC_InitTypeDef NVIC_InitStructure_USART2;
    NVIC_InitStructure_USART2.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure_USART2.NVIC_IRQChannelSubPriority        = 0x00;
    NVIC_InitStructure_USART2.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure_USART2.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure_USART2);
		
		NVIC_InitTypeDef NVIC_InitStructure_USART1;
		NVIC_InitStructure_USART1.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
		NVIC_InitStructure_USART1.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
		NVIC_InitStructure_USART1.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
		NVIC_InitStructure_USART1.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure_USART1);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void UsartConfig()
{
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	  USART1_Init(9600);
    USART2_Config();
    USART3_Config();
    USARTxNVIC_Config();
}
extern bool ariReceOk;
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void USART1_IRQHandler(void)
{
	uint8_t res = 0;
	LED_RED = ON;
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		res = USART_ReceiveData(USART1);
		v->uart->byteHandler(res);
	}
}

void USART2_IRQHandler(void)
{
	uint8_t res = 0;
	LED_RED = ON;
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
	{
		res = USART_ReceiveData(USART2);
		if(v->uart->receAir(res)) ariReceOk = true;
	}
}

void USART3_IRQHandler(void)   // ��̨�ɿ�
{
    if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
    {
			//�Ѵ���
        isACKProcessed = false;
			//���           Э���         �ֽڴ���
        isFrame = v->protocolLayer->byteHandler(USART_ReceiveData(USART3));
        if (isFrame == true)
        {                               //��ÿ��
            rFrame = v->protocolLayer->getReceivedFrame();

            //! Trigger default or user defined callback  ����Ĭ�ϻ��û�����Ļص�
					  //�����е�����
            v->processReceivedData(rFrame);

            //! Reset   ����
            isFrame        = false;
            isACKProcessed = true;
        }
    }
}


//Ӳ��������
void HardFault_Handler(void)
{
    u32 i;
    u8 t = 0;
    u32 temp;
    temp = SCB->CFSR;					//fault״̬�Ĵ���(@0XE000ED28)����:MMSR,BFSR,UFSR
    printf("CFSR:%8X\r\n", temp);	//��ʾ����ֵ
    temp = SCB->HFSR;					//Ӳ��fault״̬�Ĵ���
    printf("HFSR:%8X\r\n", temp);	//��ʾ����ֵ
    temp = SCB->DFSR;					//����fault״̬�Ĵ���
    printf("DFSR:%8X\r\n", temp);	//��ʾ����ֵ
    temp = SCB->AFSR;					//����fault״̬�Ĵ���
    printf("AFSR:%8X\r\n", temp);	//��ʾ����ֵ
    while(1);
}

#ifdef __cplusplus
}


#endif //__cplusplus
