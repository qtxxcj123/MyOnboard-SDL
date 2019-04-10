#include "led.h"
#include "sys.h"
#include "dji_vehicle.hpp"
#include "timer.h"

LedDrive::LedDrive()
{
    LED_Init();
}

LedDrive::~LedDrive()
{
}


//初始化PF9和PF10为输出口.并使能这两个口的时钟
//LED IO初始化
void  LedDrive::LED_Init(void)
{
	#if fig==0
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化

    GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10); //GPIOF9,F10设置高，灯灭
	#else 
GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟

    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1); //GPIOF9,F10设置高，灯灭
		
		#endif
}

//LED控制
//输入参数:
//颜色 , 时间
void LedDrive::LedDriveAPI(Color itp, unsigned int time)
{
    //红色
    if(red == itp)
    {
        LED1 = 1;//先关闭LED1
        LED0 = 0;//开LED0
        delay_nms(time);
    }
    //绿色
    else if(green == itp)
    {
        LED0 = 1;//先关闭LED0
        LED1 = 0;//开LED1
        delay_nms(time);
    }
    //不亮
    else
    {
        LED0 = 1;
        LED1 = 1;
        delay_nms(time);
    }
}




