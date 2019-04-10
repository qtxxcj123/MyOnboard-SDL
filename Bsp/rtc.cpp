#include "rtc.h"
/*< RTC�������� ��Ҫ�޸����ֵ >*/
#define USABLE 0xAA04 
void RtcConfig()
{
    RTC_InitTypeDef RTC_InitStructure;
    uint16_t retry = 0X1FFF;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
    PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������

    if(RTC_ReadBackupRegister(RTC_BKP_DR0) != USABLE)		//�Ƿ��һ������?
    {
        RCC_LSEConfig(RCC_LSE_ON);//LSE ����
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
        {
            retry++;
            delay_nms(10);
        }
        if(retry == 0)while(1);		//LSE ����ʧ��.

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��
        RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��

        RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F)
        RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTCͬ����Ƶϵ��(0~7FFF)
        RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
        RTC_Init(&RTC_InitStructure);

        rtcSetTime(17, 24, 0, RTC_H12_AM);	//����ʱ��
        rtcSetDate(19, 4, 1, 1);		//��������

        RTC_WriteBackupRegister(RTC_BKP_DR0, USABLE);	//����Ѿ���ʼ������
    }
}



ErrorStatus rtcSetTime(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm)
{
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;

    RTC_TimeTypeInitStructure.RTC_Hours = hour;
    RTC_TimeTypeInitStructure.RTC_Minutes = min;
    RTC_TimeTypeInitStructure.RTC_Seconds = sec;
    RTC_TimeTypeInitStructure.RTC_H12 = ampm;

    return RTC_SetTime(RTC_Format_BIN, &RTC_TimeTypeInitStructure);
}

ErrorStatus rtcSetDate(uint8_t year,uint8_t month,uint8_t date,uint8_t week)
{
    RTC_DateTypeDef RTC_DateTypeInitStructure;
    RTC_DateTypeInitStructure.RTC_Date = date;
    RTC_DateTypeInitStructure.RTC_Month = month;
    RTC_DateTypeInitStructure.RTC_WeekDay = week;
    RTC_DateTypeInitStructure.RTC_Year = year;
    return RTC_SetDate(RTC_Format_BIN, &RTC_DateTypeInitStructure);
}

