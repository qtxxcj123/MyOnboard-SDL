#ifndef __LED_H
#define __LED_H

#include "sys.h"



#if fig==1
#define LED0 PBout(1)	
#define LED1 PBout(0)	
#else 
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1
#endif

//驱动LED的类
//包含LED的初始化,LED控制
class LedDrive
{
public:
    LedDrive();
    ~LedDrive();

    typedef enum Color
    {
        red,//红色
        green,//绿色
        dark, //暗,全不亮
    } Color;

    void LED_Init(void);//初始化
    void LedDriveAPI(Color itp, unsigned int time=0);
private:
	 	//LED端口定义

};






#endif


