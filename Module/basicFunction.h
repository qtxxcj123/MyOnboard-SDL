#ifndef _BASIC_FUN_H
#define _BASIC_FUN_H
#include "myType.h"
#include "dji_mission_type.hpp"
using namespace DJI::OSDK;

//基本功能类
	/*
		包含功能：
				1、设置时间。
					传入时间相关的结构体参数
				2、读取时间。
					返回时间的结构体参数
				3、读取文件。
					读取文件的名称
					读取的文件的内容
				4、写入文件。
					写入空气传感器数据
				5、连接请求。
	*/
//TODO : 设计一个上传航点数据的方法
/*
		1、初始化航点  --  还需要上传初始化航点数据
		2、串口中保存的航点数据转化为上传时需要的航点数据
		3、上传航点数据
*/

// TODO: 初始化航点
// TODO: 设置航点参数
// ------------------------------------------OK 

//TODO : 设计一个连接功能
/*
		功能描述：用于给板载调用，确定跟上位机相连不连接不允许执行其他操作
		1、发送连接申请
		2、等待连接确定：等待连接响应的到来  --- while()
		3、返回true 确定连接通过
*/
//-------------------------------------------OK  pass-test

//TODO : 带响应的命令发送
/*
		功能描述：用于上位机需要发送命令，同时需要响应上位机命令时调用
		1、确定命令跟响应指令
		2、更新命令数组中的响应值
*/
//TODO : 提供给响应发送的数据填充
/*
		功能描述：用于下位机响应上位机指令时需要填充的一些数据 
				例如：响应空气传感器数据时，把空气传感器数据填充至发送缓存区内
*/


//TODO : 不带响应的命令发送
/*
		功能描述：发送命令指令给上位机，无响应
*/
// ------------------ 由 uart 提供
// -------------------------------------
//TODO : LED设计
/*
		功能描述：
			程序正常执行时-绿灯闪烁
			程序错误终止时-红灯常亮
			程序接发数据时-红绿闪烁
*/
//TODO : 看门狗设计 -- 考虑加入
/*
		独立看门狗 - 窗口看门狗
		使用独立看门狗---提供喂狗功能
*/
// -------------------------------------OK

class BasicFunction {
	
public:
	BasicFunction();
	~BasicFunction();

public:
	bool connect();  
	
	//TODO : 带响应的命令发送
/*
		功能描述：用于上位机需要发送命令，同时需要响应上位机命令时调用
		1、确定命令跟响应指令
		2、更新命令数组中的响应值
*/
//TODO : 提供给响应发送的数据填充
/*
		功能描述：用于下位机响应上位机指令时需要填充的一些数据 
				例如：响应空气传感器数据时，把空气传感器数据填充至发送缓存区内
*/
	void ackCmdSend(DCommand cmd,Response ack); //发送带响应的命令
	
	void saveAckData(Response ack);             //保存响应的参数- 如时间-空气传感器等

#ifdef MY_WAYPOINT
public://外部接口函数
	void uploadInfo(UartWayPointInit* uartData);
	void uploadInfo(NODE* uartData);
private://内部调用的函数
	void setWaypointInitDefaults(WayPointInitSettings* fdata);                        // 初始化 WayPointInitSettings
	void setWaypointDefaults(WayPointSettings* wp);                                   // 初始化 WayPointSettings
	void setWaypointInitParameter(UartWayPointInit* data,WayPointInitSettings* fdata);// 串口数据保存到WayPointInitSettings中
	void setWaypointParameter(NODE* data,WayPointSettings* wp);                       // 串口数据保存到WayPointSettings中
	void uploadingWaypoint(WayPointSettings* wp);								                      // 上传数据到飞控
#endif

#ifdef TIME
public:
	void setTimeAndDate(dateTime* dt); // 保存时间
	timeAndDate* getTimeAndData();
private:
	timeAndDate* myTimeAndData;
#endif
};


#endif 
