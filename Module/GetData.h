#include "dji_telemetry.hpp"
#ifndef GETDATA_H
#define GETDATA_H



using namespace DJI::OSDK;


class Get_Data
{
public:
    Get_Data();
    ~Get_Data();

    bool SubscribeBATTERY();     //订阅电池数据
    bool SubscribeGPS_FUSED();   //订阅GPS数据
    bool SubscribeGIMBAL_ANGLES();//订阅云台角度数据
    bool SubscribeFLIGHT();//订阅飞机状态


    //订阅数据 包含的数据有：
    //GPS       经度，维度，高度，卫星数量
    //电池      实时电池数据 比如 电池11.704V 返回11704
    //云台角度  x,y,z
    //飞机状态  返回0-2     0代表停止在地面,1代表飞机在地面但电机已经启动,2代表飞机已经正常起飞在空中
    bool SubscribeData();

		Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type GetGpsData();//获取GPS数据
    Telemetry::TypeMap<Telemetry::TOPIC_BATTERY_INFO>::type GetBATTERYData();//获取电池数据
    Telemetry::TypeMap<Telemetry::TOPIC_GIMBAL_ANGLES>::type GetGIMBALData();//获取云台数据
    Telemetry::TypeMap<Telemetry::TOPIC_STATUS_FLIGHT>::type GetSTATUS_FLIGHTData();//获取飞行状态数据

private:
    Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type         latLon;                          //定义GPS存储结构
    Telemetry::TypeMap<Telemetry::TOPIC_BATTERY_INFO>::type      BATTERY;                         //定义电池存储结构
    Telemetry::TypeMap<Telemetry::TOPIC_GIMBAL_ANGLES>::type     GIMBAL_ANGLES;                   //定义云台角度存储结构
    Telemetry::TypeMap<Telemetry::TOPIC_STATUS_FLIGHT>::type     STATUS_FLIGHT;                   //定义飞机状态存储结构
};


#endif














