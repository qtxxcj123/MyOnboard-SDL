/*! @file cppforstm32.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief Support for printf to USART2 on STM32 platform
 *  在stm 32平台上支持Printf to usart2
 *  @Copyright (c) 2016-2017 DJI
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
#include "cppforstm32.h"
#include "BspUsart.h"
#include "stdio.h"
#include "stdarg.h"	 
#ifdef DYNAMIC_MEMORY
void*
operator new(size_t size)
{
  if (NULL == size)
  {
#ifdef DEBUG
    printf("Error! Size is zero");
#endif // DEBUG
    return NULL;
  }
  void* p = malloc(size);
#ifdef DEBUG
  if (p == 0)
    printf("Lack Memory!");
#endif // DEBUG
  return p;
}

void*
operator new[](size_t size)
{
  return operator new(size);
}

void
operator delete(void* pointer)
{
  if (NULL != pointer)
  {
    free(pointer);
  }
}

void
operator delete[](void* pointer)
{
  operator delete(pointer);
}
#endif // DYNAMIC_MEMORY

//!@code printf link functions
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
// int fputc(int ch, FILE *f)
	//串口发送缓存区 	
	#define USART2_MAX_SEND_LEN		400					//最大发送缓存字节数
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
//串口1,printf 函数
//确保一次发送数据不超过USART1_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)//循环发送数据
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
		USART_SendData(USART2,(uint8_t)USART2_TX_BUF[j]);   
	}
}
	
	
	
#if 0
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
    USART1->DR = (u8) ch;
    return ch;
}
#endif
	
	
	
	
	
	
PUTCHAR_PROTOTYPE
{
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    ;
  USART_SendData(USART1, (uint8_t)ch);
 
  return (ch);
}
#ifdef __cplusplus
}
#endif //__cplusplus
//!@endcode printf link fuctions.
