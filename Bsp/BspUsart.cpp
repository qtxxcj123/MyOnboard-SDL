/*! @file BspUsart.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Usart helper functions and ISR for board STM32F4Discovery
 *   usart辅助功能和isr用于板的stm 32f4发现
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

//初始化IO 串口1 
//bound:波特率
void USART1_Init(unsigned int bound)
{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
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
 * USART3 is used for communicating with the DJI flight controller USART3用于与DJI飞行控制器通信
 * The Baud rate needs to match the Baud rate used by the flight controller 波特率需要与飞行控制器使用的波特率相匹配
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
		NVIC_InitStructure_USART1.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
		NVIC_InitStructure_USART1.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
		NVIC_InitStructure_USART1.NVIC_IRQChannelSubPriority =1;		//子优先级3
		NVIC_InitStructure_USART1.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure_USART1);	//根据指定的参数初始化VIC寄存器、
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

void USART3_IRQHandler(void)   // 云台飞控
{
    if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
    {
			//已处理
        isACKProcessed = false;
			//框架           协议层         字节处理
        isFrame = v->protocolLayer->byteHandler(USART_ReceiveData(USART3));
        if (isFrame == true)
        {                               //获得框架
            rFrame = v->protocolLayer->getReceivedFrame();

            //! Trigger default or user defined callback  触发默认或用户定义的回调
					  //处理中的数据
            v->processReceivedData(rFrame);

            //! Reset   重置
            isFrame        = false;
            isACKProcessed = true;
        }
    }
}


//硬件错误处理
void HardFault_Handler(void)
{
    u32 i;
    u8 t = 0;
    u32 temp;
    temp = SCB->CFSR;					//fault状态寄存器(@0XE000ED28)包括:MMSR,BFSR,UFSR
    printf("CFSR:%8X\r\n", temp);	//显示错误值
    temp = SCB->HFSR;					//硬件fault状态寄存器
    printf("HFSR:%8X\r\n", temp);	//显示错误值
    temp = SCB->DFSR;					//调试fault状态寄存器
    printf("DFSR:%8X\r\n", temp);	//显示错误值
    temp = SCB->AFSR;					//辅助fault状态寄存器
    printf("AFSR:%8X\r\n", temp);	//显示错误值
    while(1);
}

#ifdef __cplusplus
}


#endif //__cplusplus
