#include "dji_telemetry.hpp"
#ifndef GETDATA_H
#define GETDATA_H



using namespace DJI::OSDK;


class Get_Data
{
public:
    Get_Data();
    ~Get_Data();

    bool SubscribeBATTERY();     //���ĵ������
    bool SubscribeGPS_FUSED();   //����GPS����
    bool SubscribeGIMBAL_ANGLES();//������̨�Ƕ�����
    bool SubscribeFLIGHT();//���ķɻ�״̬


    //�������� �����������У�
    //GPS       ���ȣ�ά�ȣ��߶ȣ���������
    //���      ʵʱ������� ���� ���11.704V ����11704
    //��̨�Ƕ�  x,y,z
    //�ɻ�״̬  ����0-2     0����ֹͣ�ڵ���,1����ɻ��ڵ��浫����Ѿ�����,2����ɻ��Ѿ���������ڿ���
    bool SubscribeData();

		Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type GetGpsData();//��ȡGPS����
    Telemetry::TypeMap<Telemetry::TOPIC_BATTERY_INFO>::type GetBATTERYData();//��ȡ�������
    Telemetry::TypeMap<Telemetry::TOPIC_GIMBAL_ANGLES>::type GetGIMBALData();//��ȡ��̨����
    Telemetry::TypeMap<Telemetry::TOPIC_STATUS_FLIGHT>::type GetSTATUS_FLIGHTData();//��ȡ����״̬����

private:
    Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type         latLon;                          //����GPS�洢�ṹ
    Telemetry::TypeMap<Telemetry::TOPIC_BATTERY_INFO>::type      BATTERY;                         //�����ش洢�ṹ
    Telemetry::TypeMap<Telemetry::TOPIC_GIMBAL_ANGLES>::type     GIMBAL_ANGLES;                   //������̨�Ƕȴ洢�ṹ
    Telemetry::TypeMap<Telemetry::TOPIC_STATUS_FLIGHT>::type     STATUS_FLIGHT;                   //����ɻ�״̬�洢�ṹ
};


#endif














