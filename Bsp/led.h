#ifndef _LED_H
#define _LED_H
#include "stm32f4xx.h"
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)

#define LED_RED   PBout(1)    // ºìµÆ
#define LED_GREEN PBout(0)    // ÂÌµÆ

#define ON        0
#define OFF       1

void ledInit(void);
#endif
