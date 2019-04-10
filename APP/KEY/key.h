#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
 
//驱动按键的类
//包含按键的初始化,按键检测
class KeyDrive
{
public:
    KeyDrive();
    ~KeyDrive();

    void KEY_Init(void);//初始化
    int Get_Key_val();
private:
//按键端口定义
#define KEY 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) //PA4

};





#endif
