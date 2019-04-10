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
	   //删除订阅包
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


//订阅GPS数据  标志0 50HZ
//数据 经度 维度 高度 卫星数量
//返回true 订阅成功          其它：失败
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

//订阅电池数据  标志1 50HZ
//返回true 订阅成功          其它：失败
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

//订阅云台角度数据  标志2 50HZ
//返回true 订阅成功          其它：失败
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


//订阅飞机状态  标志3 50HZ
//返回true 订阅成功          其它：失败
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


//订阅数据 包含的数据有：
//GPS       经度，维度，高度，卫星数量
//电池      实时电池数据 比如 电池11.704V 返回11704
//云台角度  x,y,z
//飞机状态  返回0-2     0代表停止在地面,1代表飞机在地面但电机已经启动,2代表飞机已经正常起飞在空中
bool Get_Data::SubscribeData()
{
    if (v->getFwVersion() != Version::M100_31)
    {
        //订阅GPS数据  标志0 50HZ
        //数据 经度 维度 高度 卫星数量
        //返回true 订阅成功          其它：失败
        if (!SubscribeGPS_FUSED())
        {
            printf("GPS Subscribe ERROR\n\r");
            return false;
        }
        delay_nms(1000);

        //订阅电池数据  标志1 50HZ
        //返回true 订阅成功          其它：失败
        if (!SubscribeBATTERY())
        {
            printf("BATTERY Subscribe ERROR\n\r");
            return false;
        }
        delay_nms(1000);
				
        //订阅云台角度数据  标志2 50HZ
        //返回true 订阅成功          其它：失败
        if (!SubscribeGIMBAL_ANGLES())
        {
            printf("GIMBAL_ANGLES Subscribe ERROR\n\r");
            return false;
        }
        delay_nms(1000);
				
        //订阅飞机状态  标志3 50HZ
        //返回true 订阅成功          其它：失败
        if (!SubscribeFLIGHT())
        {
            printf("FLIGHT_ANGLES Subscribe ERROR\n\r");
            return false;
        }
        delay_nms(1000);
    }
}

//获取GPS数据
TypeMap<TOPIC_GPS_FUSED>::type 
Get_Data::GetGpsData()
{
  latLon  = v->subscribe->getValue<Telemetry::TOPIC_GPS_FUSED>();  
	return latLon;
}

//获取电池数据
TypeMap<TOPIC_BATTERY_INFO>::type 
Get_Data::GetBATTERYData()
{
  BATTERY  = v->subscribe->getValue<Telemetry::TOPIC_BATTERY_INFO>(); 
	return BATTERY;
}

//获取云台数据
TypeMap<TOPIC_GIMBAL_ANGLES>::type 
Get_Data::GetGIMBALData()
{
  GIMBAL_ANGLES  = v->subscribe->getValue<Telemetry::TOPIC_GIMBAL_ANGLES>();  
	return GIMBAL_ANGLES;
}

//获取飞机状态数据
TypeMap<TOPIC_STATUS_FLIGHT>::type 
Get_Data::GetSTATUS_FLIGHTData()
{
  STATUS_FLIGHT  = v->subscribe->getValue<Telemetry::TOPIC_STATUS_FLIGHT>(); 
	return STATUS_FLIGHT;
}










