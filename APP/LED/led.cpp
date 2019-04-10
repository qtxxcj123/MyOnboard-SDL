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


//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��
//LED IO��ʼ��
void  LedDrive::LED_Init(void)
{
	#if fig==0
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

    //GPIOF9,F10��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��

    GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10); //GPIOF9,F10���øߣ�����
	#else 
GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOFʱ��

    //GPIOF9,F10��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1); //GPIOF9,F10���øߣ�����
		
		#endif
}

//LED����
//�������:
//��ɫ , ʱ��
void LedDrive::LedDriveAPI(Color itp, unsigned int time)
{
    //��ɫ
    if(red == itp)
    {
        LED1 = 1;//�ȹر�LED1
        LED0 = 0;//��LED0
        delay_nms(time);
    }
    //��ɫ
    else if(green == itp)
    {
        LED0 = 1;//�ȹر�LED0
        LED1 = 0;//��LED1
        delay_nms(time);
    }
    //����
    else
    {
        LED0 = 1;
        LED1 = 1;
        delay_nms(time);
    }
}




