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

//����LED����
//����LED�ĳ�ʼ��,LED����
class LedDrive
{
public:
    LedDrive();
    ~LedDrive();

    typedef enum Color
    {
        red,//��ɫ
        green,//��ɫ
        dark, //��,ȫ����
    } Color;

    void LED_Init(void);//��ʼ��
    void LedDriveAPI(Color itp, unsigned int time=0);
private:
	 	//LED�˿ڶ���

};






#endif


