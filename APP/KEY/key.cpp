#include "key.h"
#include "timer.h"


KeyDrive::KeyDrive()
{
    KEY_Init();
}

KeyDrive::~KeyDrive()
{

}


//������ʼ������
void KeyDrive::KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOA,GPIOEʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

   

}
//��������
//���ذ���ֵ
int KeyDrive::Get_Key_val()
{
    if(KEY == 0)
    {
        delay_nms(5);//ȥ����
        if(KEY == 0)return 1;
    }
    return 0;// �ް�������
}












