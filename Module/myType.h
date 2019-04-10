#ifndef _MYTYPE_H
#define _MYTYPE_H
#include <stdint.h>
#include <string.h>
#include "crc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "ff.h"
#define ARRAY_SIZE 10
#define MY_WAYPOINT
#define MY_ARI
//namespace myTypedef {

//----------------------------------------------------------------------------------------------------
//接收相关的结构


typedef enum Ecommand { //下位机接收到的指令类型
	UP_NONE_COMMAND = 0x00,				/*< 上位机命令：无命令>*/
	UP_STATUS_COMMAND = 0x01,			/*< 上位机命令：请求状态命令>*/
	UP_AIR_FEI_COMMAND = 0x02,		/*< 上位机命令：请求空气传感器和飞机状态命令>*/
	UP_STOP_COMMAND = 0x03,				/*< 上位机命令：请求停止命令>*/
	UP_AIR_COMMAND = 0x04					/*< 上位机命令：请求空气传感器命令>*/
}Ecommand;

typedef enum Eresponse {//下位机接收到的响应类型
	UP_NONE_ACK = 0x00,						/*< 上位机响应：无响应 - 对应下位机无命令>*/
	UP_CONNECT_ACK = 0x01,				/*< 上位机响应：连接响应 - 对应下位机连接命令>*/
	UP_TIME_ACK = 0x02,						/*< 上位机响应：时间响应 - 对应下位机请求时间命令>*/
	UP_WAYPOINT_ACK = 0x03,				/*< 上位机响应：航点响应 - 对应下位机请求航点命令>*/
	UP_ERROR_ACK = 0xEE						/*< 上位机响应：错误响应 - 对应下位机发送错误命令>*/
}Eresponse;

typedef enum EError {//错误类型
	RECE_OK = 0,		/*< 接收成功：表示接收完成 >*/
//	NONE,						/*< 没有状态：表示没有任何情况 >*/
	HEAD_ERROR,			/*< 头错误：表示接收到了错误的帧头 >*/												
	TRAIL_ERROR,		/*< 尾错误：表示接收到了错误的帧尾 >*/		
	CRC_ERROR,			/*< CRC错误：表示CRC校验出现错误 >*/	
	NULL_ERROR,			/*< 空错误：表示现在无状态 - 初始时的状态 >*/	
	COMMAND_ERROR,	/*< 连接错误：表示连接出现了错误 >*/	
	RESPONSE_ERROR	/*< 响应错误：表示出现了响应错误 >*/	
}EError;

typedef struct uartBasicInfo {
	uint8_t index;			//数据的索引，接收到整帧数据+1			
	uint16_t length;		//数据的长度，就收到数据+1
	uint8_t* buffer;		//数据缓存区		
	bool savaDataFlg;		//是否需要保存数据
	Ecommand isCommand;		//指令集	
	Eresponse isResponse;   //响应集 
	EError error;			//错误类型
}uartBasicInfo;

typedef struct SdataHead {
	uint8_t head : 8;		/*< 包头	 >*/
	uint8_t len : 8;		/*< 数据长度 >*/
	uint8_t command : 8;	/*< 命令集	 >*/
	uint8_t response : 8;	/*< 响应集   >*/
}SdataHead;


typedef struct UartWayPointInit {		  // 航点初始化接收结构体
	uint8_t speedB : 4;					  // 初始化速度
	uint8_t speedD : 4;					  // 初始化速度
	uint8_t yawMode : 4;				  // 偏航模式
	uint8_t endAction : 4;				  // 结束动作
	uint8_t heightH : 8;				  // 高度（高位）
	uint8_t heightL : 8;				  // 高度（低位）
	uint8_t wayNumber : 8;				  // 航点总数	
}UartWayPointInit;

typedef struct UartWayPointData {			//航点任务接收结构体
	uint8_t latitudeLength : 4;				// 维度长度
	uint8_t longitudeLength : 4;			// 经度长度
	uint8_t heightH : 8;				    // 高度高
	uint8_t heightL : 8;				    // 高度低
	uint8_t turnMode : 4;				    // 转动模式
	uint8_t yawPM : 4;						// 偏航正负
	uint8_t yaw : 8;						// 偏航角
	uint8_t actionFlag : 4;					// 动作标志
	uint8_t actionTime : 4;					// 动作次数
	uint8_t actionNumber : 8;				// 动作个数
}UartWayPointData;

typedef struct dateTime {
	uint8_t year;        //12
	uint8_t month;        //月
	uint8_t date;          //日
	uint8_t week;		  //星期
	uint8_t hour;         //时
	uint8_t minute;       //分
	uint8_t second;       //秒
}dateTime;

typedef struct PreDim {
	uint8_t longitude[11];				// 精度
	uint8_t latitude[10];				  // 维度
}PreDim;

typedef struct Actions {
	uint8_t actionCommand[16];			  // 动作命令
	uint8_t actionParameter[16];			// 动作参数
}Actions;

typedef struct waypoint {
	UartWayPointData* uartWayData;//节点数据
	PreDim		jwData;	//经纬度数据
	Actions		action;	//动作数据
}waypoint;
// TODO 改为指针

typedef struct NODE {	//节点
	uint8_t id;			//节点id
	waypoint* data;		//航点任务数据
	NODE* next;			//下个节点
}NODE;

typedef struct waypointManage {
	bool isInitWaypint; //航点任务是否被初始化过
	uint8_t countWaypoint; //记录保存的航点个数
	uint8_t saveMaxWaypoint; //记录最大的航点个数
	uint8_t saveLastWaypoint; //记录上次航点的个数
}waypointManage;


typedef struct Ari{
   uint16_t co2 : 16;
   uint16_t voc : 16;
   uint16_t hum : 16;
   uint16_t temp : 16;
   uint16_t pm2_5 : 16;
}Ari;



//---------------------------------------------------------------------------------------
//发送相关的结构

typedef struct SendBufferOne {                                           /*< 一次数据 >*/
	//uint8_t                                                  head = 0xFD;	 /*< 包头 >*/		
	// Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type     latLon;       /*< GPS >*/
	// char*                                                    SerialNum;    /*< 版本号 >*/ 
}SendBufferOne;


typedef struct SendBuffer {
	//uint8_t                                                     head = 0xFD;  /*< 包头 >*/		
	// Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type        latLon;       /*< GPS  >*/
	// Telemetry::TypeMap<Telemetry::TOPIC_BATTERY_INFO>::type     battery;      /*< 电池 >*/
	// Telemetry::TypeMap<Telemetry::TOPIC_GIMBAL_ANGLES>::type    gimbal;       /*< 云台 >*/
	// Telemetry::TypeMap<Telemetry::TOPIC_STATUS_FLIGHT>::type    flight;       /*< 飞机 >*/
}SendBuffer;

typedef struct sendCommand { 
	uint8_t* noneCmd;		//无指令
	uint8_t* connectCmd;	//连接/判断指令
	uint8_t* timeCmd;		//时间指令
	uint8_t* waypointCmd;	//航点指令
}sendCommand;

typedef enum DCommand { //下位机发送的指令类型
	DOWN_NONE_CMD = 0x00,				/*< 下位机命令：无命令 >*/
	DOWN_CONNECT_CMD = 0x01,		/*< 下位机命令：请求连接命令 >*/
	DOWN_TIME_CMD = 0x02,				/*< 下位机命令：请求时间命令 >*/
	DOWN_WAYPOINT_CMD = 0x03		/*< 下位机命令：请求航点命令 >*/
}DCommand;

typedef enum Response { //下位机发送的响应类型
	DOWN_NONE_ACK = 0x00,				/*< 下位机响应：无响应 - 对应上位机无命令 >*/
	DOWN_INIT_ACK = 0x01,				/*< 下位机响应：初始化响应 - 对应上位机请求飞控初始化状态命令 >*/
	DOWN_ARI_FEI_ACK = 0x02,		/*< 下位机响应：空气传感器和飞机响应 -对应上位机飞机状态和空气传感器数据命令 >*/
	DOWN_STOP_ACK = 0x03,				/*< 下位机响应：暂停响应 - 对应上位机请求暂停（航点）命令 >*/
	DOWN_ARI_ACK = 0x04,				/*< 下位机响应：空气传感器响应 - 对应上位机空气传感器命令 >*/
	DOWN_ERROR = 0xEE						/*< 下位机响应：错误响应 - 对应上位机发送了错误的命令响应 >*/
}Response;


const uint8_t noneCmd[ARRAY_SIZE] =		{ 0xFF,0x0A,0x00,0x00,0x43,0x4E,0x44,0xE2,0xD0,0xBB };   	/*< 下位机空命令固定数据格式 >*/
const uint8_t connectCmd[ARRAY_SIZE] =	{ 0xFF,0x0A,0x01,0x00,0x44,0x4E,0x44,0xE3,0x5C,0xBB }; 	/*< 下位机请求连接固定数据格式 >*/
const uint8_t timeCmd[ARRAY_SIZE] =		{ 0xFF,0x0A,0x02,0x00,0x45,0x4E,0x44,0x23,0x49,0xBB };		/*< 下位机请求空气传感器数据固定数据格式 >*/
const uint8_t waypointCmd[ARRAY_SIZE] = { 0xFF,0x0A,0x03,0x00,0x46,0x4E,0x44,0xE3,0x84,0xBB };	/*< 下位机请求航点任务固定格式 >*/



const uint8_t startAriArray[] = {0x11, 0x02, 0x01, 0x00, 0xEC};		/*< 请求开始空气传感器数据回传固定数据格式 >*/

//基础功能相关类的结构体定义
//---基础功能时间基础结构
typedef struct timeAndDate {
	RTC_TimeTypeDef* time;	/*< RTC时间结构体 >*/
	RTC_DateTypeDef* date;  /*< RTC日期结构体 >*/
}timeAndDate;

//}
#endif
