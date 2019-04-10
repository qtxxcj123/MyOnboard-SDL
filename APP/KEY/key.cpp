#include "key.h"
#include "timer.h"


KeyDrive::KeyDrive()
{
    KEY_Init();
}

KeyDrive::~KeyDrive()
{

}


//按键初始化函数
void KeyDrive::KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA,GPIOE时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOE2,3,4

   

}
//按键处理
//返回按键值
int KeyDrive::Get_Key_val()
{
    if(KEY == 0)
    {
        delay_nms(5);//去抖动
        if(KEY == 0)return 1;
    }
    return 0;// 无按键按下
}












