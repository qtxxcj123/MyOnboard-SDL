#ifndef _RTC_H
#define _RTC_H
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"
#include "timer.h"
/* 初始化RTC */
void RtcConfig();
/* 设置rtc时间 */
ErrorStatus rtcSetTime(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);
/* 设置rtc日期 */
ErrorStatus rtcSetDate(uint8_t year,uint8_t month,uint8_t date,uint8_t week);

void RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt);
#endif
