#ifndef _BUFFER_H
#define _BUFFER_H
#include "myType.h"
#include "ackTools.h"
//#include "listLinked.h"
#include "dma.h"
#include "list.h"

//////////////////////////////////////////////////////////////////////////
/*
下位机指令集：
0x00：无指令
0x01：请求连接
0x02：请求时间数据
0x03：请求航点任务

下位机响应集：
0x00：无响应
0x01：返回飞机初始化状态（一次数据，版本号）
0x02：返回飞机状态和空气传感器数据
0x03：返回暂停 OK
0x04：返回空气传感器数据
0xEE：响应错误

上位机指令集：
0x00：无指令
0x01：请求飞机初始化状态（一次数据，版本号）
0x02：请求飞机状态和空气传感器数据
0x03：请求暂停航点任务
0x04：请求空气传感器数据

上位机响应集：
0x00：无响应
0x01：连接确认
0x02：返回时间数据
0x03：返回航点任务
0xEE：响应错误
*/
//////////////////////////////////////////////////////////////////////////


//#define UP_NONE_COMMAND			0x00		/*< 无指令	 >*/
//#define UP_STATUS_COMMAND		0x01		/*< 状态指令 >*/
//#define UP_STOP_COMMAND			0x02		/*< 暂停指令 >*/
//#define UP_AIR_COMMAND			0x03		/*< 空气指令 >*/

//#define UP_OK_ACK				0x00		/*< OK响应   >*/
//#define UP_CONNECT_ACK			0x01		/*< 连接响应 >*/
//#define UP_TIME_ACK				0x02		/*< 时间响应 >*/
//#define UP_WAYPOINT_ACK			0x03		/*< 航点响应 >*/
//#define UP_ERROR_ACK			0x04		/*< 错误响应 >*/

#define WAYPOINT_INIT_LENGHT 12


class UartBuffer {
public:
	UartBuffer();
	~UartBuffer();
public:
	/*
	函数原型：void init();
	函数参数：无
	函数返回：无
	函数作用：初始化一些成员
	*/
	void init();
	/*
	函数原型：void headInit();
	函数参数：无
	函数返回：无
	函数作用：初始化头指针的成员
	*/
	void headInit();

	/*
	函数原型：void wayInit();
	函数参数：无
	函数返回：无
	函数作用：初始化航点的成员
	*/
	void wayInit();

	/*
	函数原型：void ariInit();
	函数参数：无
	函数返回：无
	函数作用：初始化空气传感器的成员
	*/
	void ariInit();
	
	/*
	函数原型：bool byteHandle(const uint8_t data);
	函数参数：data：接收到的串口数据
	函数返回：True | False
	函数说明：接收串口数据
	*/
	bool byteHandler(const uint8_t data);	//step 1

	/*
	函数原型：void storeDataBuffer(const uint8_t data);
	函数参数：data：需要保存的数据
	函数返回：无
	函数说明：保存数据到缓存区
	*/
	bool storeDataBuffer(const uint8_t data);	//step 2

	/*
	函数原型：bool checkDataStream();
	函数参数：无
	函数返回：True | False
	函数说明：检查数据流
	*/

	bool checkDataStream();				//step 3

	/*
	函数原型：void isCRC(uint8_t* data,uint8_t len);
	函数参数：data：需要效验的数据	len：数据长度
	函数返回：True | False
	函数说明：效验CRC
	*/
	//bool isCRC(uint8_t* data,uint8_t len);

	/*
	函数原型：bool verifyHead();
	函数参数：无
	函数返回：True | False
	函数说明：验证头
	*/
	bool verifyHead();
	
	/*
	函数原型：bool verifyData();
	函数参数：无
	函数返回：True | False
	函数说明：验证数据
	*/
  
	bool verifyData();

	/*
	函数原型：void verifyCommand();
	函数参数：无
	函数返回：无
	函数说明：验证命令
	*/
	void verifyCommand();

	/*
	函数原型：void verifyResponse();
	函数参数：无
	函数返回：无
	函数说明：验证响应
	*/
	void verifyResponse();

	/*
	函数原型：void executiveCommand();
	函数参数：无
	函数返回：无
	函数说明：执行指令
	*/
	void executiveCommand();

	/*
	函数原型：void clearResponse();
	函数参数：无
	函数返回：无
	函数说明：清楚一个响应
	*/
	void clearResponse();

	/*
	函数原型：void saveTimeData(void* source);
	函数参数：dataAddr：源数据地址
	函数返回：无
	函数说明：保存时间数据
	*/
	void saveTimeData(void* dataAddr);
#ifdef MY_WAYPOINT	
	/*
	函数原型：void saveTimeData(void* source);
	函数参数：dataAddr：源数据地址
	函数返回：无
	函数说明：保存航点数据
	*/
	void saveWaypoint(void* dataAddr);

	/*
	函数原型：void remove_nm(char * p, int m, int n, uint8_t * save);
	函数参数：p：原地址  m：删除首地址偏移量   n：删除个数   save：删除数据保存地址
	函数返回：无
	函数功能：删除数组中的数据保存到另一个数组中
	*/
	void remove_nm(char * p, int m, int n, uint8_t * save);

	/*
	函数原型：initWapoint
	函数参数：无
	函数返回：无
	函数功能：一次航点数据全部结束后清空管理器
	*/
	void initWapoint();
	
	/*
	函数原型：waypointManage* getWayManage()
	函数参数：无
	函数返回：uint8_t
	函数说明：返回航点数目
	*/
	// 提供给外界航点的管理器，用于查看航点接受的状态
	uint8_t getSaveWaypoint();
	
	UartWayPointInit getInitWaypoint();
#endif
	// --------------------------------------------------------
	// 发送相关
	/*
	函数原型：void send(USART_TypeDef* USARTx,void* data,uint8_t len);
	函数参数：USARTx：发送到那个串口 data：待发送的数据	len：数据长度
	函数返回：无
	函数说明：发送数据到串口
	*/
	void send(USART_TypeDef* USARTx,const void* data, uint8_t len);

	/*
	函数原型：void sendCmd(Command cmd);
	函数参数：cmd：指令枚举
	函数返回：无
	函数说明：发送指令
	*/
	// 发送一个指令会挂起一个响应，等待接收响应，清空
	void sendCmd(DCommand cmd);
	
	/*
	函数原型：void sendDma(void* data,uint8_t len)
	函数参数：data：待发送数据	len：发送长度
	函数返回：无
	函数说明：发送数据到DMA
	*/
	void sendDma(void* data,uint8_t len);

	/*
	函数原型：void setPutUp(Command cmd);
	函数参数：cmd：挂起响应的类型
	函数返回：无
	函数说明：挂起一个响应
	*/
	void setPutUp(DCommand cmd);
  
#ifdef MY_ARI
  /*
	函数原型：void receAir(const uint8_t data);
	函数参数：data：空气传感器数据
	函数返回：true | false
	函数说明：接受空气传感器数据
	*/
  bool receAir(const uint8_t data);
    
  /*
	函数原型：void ariHandler();
	函数参数：无
	函数返回：true | false
	函数说明：处理空气传感器数据
	*/
  bool ariHandler();
	
  /*
	函数原型：void verifyAriHead();
	函数参数：无
	函数返回：无
	函数说明：效验空气传感器数据头
	*/
	void verifyAriHead();
	
  /*
	函数原型：void verifyAri();
	函数参数：无
	函数返回：true | false
	函数说明：效验空气传感器的数据
	*/
  bool verifyAri();
	
  /*
	函数原型：ari getAri();
	函数参数：无
	函数返回：ari
	函数说明：返回空气传感器数据
	*/
	Ari* getAri();
	
	/*
	函数原型：saveAriData(uint8_t* prdAri)
	函数参数：prdAri
	函数返回：无
	函数说明：保存空气传感器数据
	*/
	void saveAriData(uint8_t* prdAri);
	
	/*
	函数原型：stratAri()
	函数参数：无
	函数返回：无
	函数说明：开启空气传感器上传（无法关闭）
	*/
	void startAri();
	
	/*
		获取一些成员的函数与设置一些成员的函数
	*/
#endif
	
	/*
	函数原型：getCmdFlg
	函数参数：无
	函数返回：true | false
	函数说明：返回给外界是否有需要执行的命令
	*/
	bool getCmdFlg();		// 提供给外界轮询命令
	/*
	函数原型：setCmdFlg(bool b)
	函数参数：b：清除
	函数返回：true | false
	函数说明：返回给外界是否有需要执行的命令
	*/
	void clearCmdFlg();	// 提供给外界清除任务标志
	/*
	函数原型：getCommand()
	函数参数：无
	函数返回：Ecommand
	函数说明：返回给外界一个确定的指令集（具体需要执行那条指令）
	*/
	Ecommand getCommand();	// 提供给外界确定的命令
	/*
	函数原型：getEError()
	函数参数：无
	函数返回：EError
	函数说明：返回给外界错误状态（包括接收完成）
	*/
	EError getEError();   // 提供现在error状态
	/*
	函数原型：clearEError()
	函数参数：无
	函数返回：无
	函数说明：返回给外界清空错误的接口
	*/
	void clearEError();
	
	/*
	函数原型：getAck()
	函数参数：无
	函数返回：ackTools*
	函数说明：返回给外界响应类工具
	*/
	ackTools* getAck();

private:  // 跟头相关的成员
	uartBasicInfo* uHead;		/*< 串口基本信息-包含一些必备信息[数据的个数索引 - 本帧数据的长度 - 数据缓存区 - 等]>*/
	uint8_t* prdData;				/*< 指向数据区的指针 >*/			
	ackTools* ack;					/*< ack应答类的指针  用于确保响应等到清除 >*/
	bool cmdFlg;						/*< 提供给外界判断是否有需要执行任务的标志位 >*/
	const uint8_t BUFF_MAX; /*< 缓存区的最大值 >*/
#ifdef MY_WAYPOINT
private://航点相关成员
	waypointManage* wayManage;		  /*< 航点管理器 管理航点的基本信息 - 航点个数等>*/
	UartWayPointInit initWaypoint; /*< 用于接收航点初始化的指针 >*/
	NODE*	waypointSetting;					/*< 链表类型  通过该成员把航点数据添加到链表 >*/
#endif
#ifdef TIME
private://时间相关成员
	dateTime* date;			/* 用于保存时间的指针 */
#endif


#ifdef MY_ARI
	Ari* pMyAri;						/*< 保存空气传感器数据指针 >*/
	uint8_t* ariBuff;				/*< 空气传感器数据缓存区 >*/	
	uint8_t* prdAri;				/*< 指向空气传感器数据的指针 >*/
	uint8_t ariCount;				/*< 空气传感器计数值 >*/
	const uint8_t ARI_MAX;	/*< 空气传感器数据最大值 >*/
#endif
};


#endif
