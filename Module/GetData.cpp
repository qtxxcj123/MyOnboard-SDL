#include "GetData.h"
#include "timer.h"
#include "stdio.h"

extern Vehicle  vehicle;
extern Vehicle *v;

using namespace DJI::OSDK::Telemetry;

Get_Data::Get_Data()
{
//SubscribeData();
}

Get_Data::~Get_Data()
{
	   //ɾ�����İ�
     v->subscribe->removePackage(0);
		 delay_nms(1000);
		 v->subscribe->removePackage(1);
		 delay_nms(1000);
		 v->subscribe->removePackage(2);
		 delay_nms(1000);
		 v->subscribe->removePackage(3);
		 delay_nms(1000);
	   v->subscribe->removePackage(4);
		 delay_nms(1000);
}


//����GPS����  ��־0 50HZ
//���� ���� ά�� �߶� ��������
//����true ���ĳɹ�          ������ʧ��
bool Get_Data::SubscribeGPS_FUSED()
{
    int                  pkgIndex        = 0;
    int                  freq            = 50;
    TopicName topicList50Hz[]  = { TOPIC_GPS_FUSED };

    int  numTopic        = sizeof(topicList50Hz) / sizeof(topicList50Hz[0]);
    bool enableTimestamp = false;

    bool pkgStatus = v->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList50Hz, enableTimestamp, freq);
    if (!(pkgStatus))
    {
        return pkgStatus;
    }
    v->subscribe->startPackage(pkgIndex);
    delay_nms(10);
    return true;
}

//���ĵ������  ��־1 50HZ
//����true ���ĳɹ�          ������ʧ��
bool Get_Data::SubscribeBATTERY()
{
    int                  pkgIndex        = 1;
    int                  freq            = 50;
    TopicName topicList50Hz[]  = { TOPIC_BATTERY_INFO };

    int  numTopic        = sizeof(topicList50Hz) / sizeof(topicList50Hz[0]);
    bool enableTimestamp = false;

    bool pkgStatus = v->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList50Hz, enableTimestamp, freq);
    if (!(pkgStatus))
    {
        return pkgStatus;
    }
    v->subscribe->startPackage(pkgIndex);
    delay_nms(10);
    return true;
}

//������̨�Ƕ�����  ��־2 50HZ
//����true ���ĳɹ�          ������ʧ��
bool Get_Data::SubscribeGIMBAL_ANGLES()
{
    int                  pkgIndex        = 2;
    int                  freq            = 50;
    TopicName topicList50Hz[]  = { TOPIC_GIMBAL_ANGLES };

    int  numTopic        = sizeof(topicList50Hz) / sizeof(topicList50Hz[0]);
    bool enableTimestamp = false;

    bool pkgStatus = v->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList50Hz, enableTimestamp, freq);
    if (!(pkgStatus))
    {
        return pkgStatus;
    }
    v->subscribe->startPackage(pkgIndex);
    delay_nms(10);
    return true;
}


//���ķɻ�״̬  ��־3 50HZ
//����true ���ĳɹ�          ������ʧ��
bool Get_Data::SubscribeFLIGHT()
{
    int                  pkgIndex        = 3;
    int                  freq            = 50;
    TopicName topicList50Hz[]  = { TOPIC_STATUS_FLIGHT  };

    int  numTopic        = sizeof(topicList50Hz) / sizeof(topicList50Hz[0]);
    bool enableTimestamp = false;

    bool pkgStatus = v->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList50Hz, enableTimestamp, freq);
    if (!(pkgStatus))
    {
        return pkgStatus;
    }
    v->subscribe->startPackage(pkgIndex);
    delay_nms(10);
    return true;
}


//�������� �����������У�
//GPS       ���ȣ�ά�ȣ��߶ȣ���������
//���      ʵʱ������� ���� ���11.704V ����11704
//��̨�Ƕ�  x,y,z
//�ɻ�״̬  ����0-2     0����ֹͣ�ڵ���,1����ɻ��ڵ��浫����Ѿ�����,2����ɻ��Ѿ���������ڿ���
bool Get_Data::SubscribeData()
{
    if (v->getFwVersion() != Version::M100_31)
    {
        //����GPS����  ��־0 50HZ
        //���� ���� ά�� �߶� ��������
        //����true ���ĳɹ�          ������ʧ��
        if (!SubscribeGPS_FUSED())
        {
            printf("GPS Subscribe ERROR\n\r");
            return false;
        }
        delay_nms(1000);

        //���ĵ������  ��־1 50HZ
        //����true ���ĳɹ�          ������ʧ��
        if (!SubscribeBATTERY())
        {
            printf("BATTERY Subscribe ERROR\n\r");
            return false;
        }
        delay_nms(1000);
				
        //������̨�Ƕ�����  ��־2 50HZ
        //����true ���ĳɹ�          ������ʧ��
        if (!SubscribeGIMBAL_ANGLES())
        {
            printf("GIMBAL_ANGLES Subscribe ERROR\n\r");
            return false;
        }
        delay_nms(1000);
				
        //���ķɻ�״̬  ��־3 50HZ
        //����true ���ĳɹ�          ������ʧ��
        if (!SubscribeFLIGHT())
        {
            printf("FLIGHT_ANGLES Subscribe ERROR\n\r");
            return false;
        }
        delay_nms(1000);
    }
}

//��ȡGPS����
TypeMap<TOPIC_GPS_FUSED>::type 
Get_Data::GetGpsData()
{
  latLon  = v->subscribe->getValue<Telemetry::TOPIC_GPS_FUSED>();  
	return latLon;
}

//��ȡ�������
TypeMap<TOPIC_BATTERY_INFO>::type 
Get_Data::GetBATTERYData()
{
  BATTERY  = v->subscribe->getValue<Telemetry::TOPIC_BATTERY_INFO>(); 
	return BATTERY;
}

//��ȡ��̨����
TypeMap<TOPIC_GIMBAL_ANGLES>::type 
Get_Data::GetGIMBALData()
{
  GIMBAL_ANGLES  = v->subscribe->getValue<Telemetry::TOPIC_GIMBAL_ANGLES>();  
	return GIMBAL_ANGLES;
}

//��ȡ�ɻ�״̬����
TypeMap<TOPIC_STATUS_FLIGHT>::type 
Get_Data::GetSTATUS_FLIGHTData()
{
  STATUS_FLIGHT  = v->subscribe->getValue<Telemetry::TOPIC_STATUS_FLIGHT>(); 
	return STATUS_FLIGHT;
}










