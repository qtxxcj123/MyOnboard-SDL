#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
 
//������������
//���������ĳ�ʼ��,�������
class KeyDrive
{
public:
    KeyDrive();
    ~KeyDrive();

    void KEY_Init(void);//��ʼ��
    int Get_Key_val();
private:
//�����˿ڶ���
#define KEY 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) //PA4

};





#endif
