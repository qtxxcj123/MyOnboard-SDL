/*! @file main.cpp
 *  @version 3.3
 *  @date May 2017
 *
 *  @brief
 *  An exmaple program of DJI-onboard-SDK portable for stm32
 *  用于STM32的dji-board-sdk的退出程序
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 *
 *******************************************************************************
 *                                                                             *
 *          --------               --------                 --------           *
 *         |        |   USART2    |        |    USART3     |        |          *
 *         |   PC   | <---------> | stm32  |  <----------> |  M100  |          *
 *         |        | (USB-TTL)   |        |               |        |          *
 *         |        |             |        |               |        |          *
 *          --------               --------                 --------           *
 *                                                                             *
 *                                                                             *
 *******************************************************************************
 * */
//    -DFLIGHT_CONTROL_SAMPLE
#include "stm32f4xx.h"
#include "main.h"
#include "BspUsart.h"
#include "stdio.h"
#ifdef USER_DEBUG
#define MY_DEBUG(format,...) printf("FILE: "__FILE__", LINE: %d: "format, __LINE__, ##__VA_ARGS__)
#else
#define MY_DEBUG(format,...)
#endif

/*-----------------------DJI_LIB VARIABLE-----------------------------*/
using namespace DJI::OSDK;
//线程支持
bool           threadSupport = false;
//框架
bool           isFrame       = false;
RecvContainer  receivedFrame;//收到框架
RecvContainer *rFrame  = &receivedFrame;
// 车辆                                线程支持
Vehicle        vehicle = Vehicle(threadSupport);
Vehicle       *v       = &vehicle;

//**********************************
/*variable definitely*/
bool ariReceOk = false;
static bool droneInit(void);
static void ledHandler(void);
static void initFatfs(void);
static void saveAriToSd(void);
static void waypointUploading(void);
static void renewFatfs(void);
static char buff[128];
const char* fileName = "0:/newDataFile.txt";
FIL fil;
UINT bww;
FATFS fs;
FRESULT res;
//**********************************
void 
sendWaypoint(NODE* data)
{
	char buff[64];
	uint8_t count = 0;
	count += sprintf(&buff[count],"id=%d\r\n",data->id);
	count += sprintf(&buff[count], "j=%s\r\n", data->data->jwData.longitude);
	count += sprintf(&buff[count], "w=%s\r\n", data->data->jwData.latitude);
	count += sprintf(&buff[count], "h=%d\r\n", data->data->uartWayData->heightH* 256
		+ data->data->uartWayData->heightL);
	count += sprintf(&buff[count], "num=%d\r\n", data->data->uartWayData->actionNumber);
	v->uart->send(USART1,buff,count);
}


int main(void)
{
    BSPinit(); 
		delay_nms(500);
	  //droneInit();
		v->functionalSetUp();
		delay_nms(500);
		//v->MySubscribe->SubscribeData();
		//delay_nms(500);
		initFatfs();
		delay_nms(500);
    v->uart->startAri();
		delay_nms(500);
    while (1)
    {
        if(!res && ariReceOk)  // TODO : 空气传感器接收存在 bug
        {
            ariReceOk = false;
            saveAriToSd();
        }
				if(!v->uart->getEError()) // 接收完成
        {
				// TODO: 这里需要继续添加上位机响应集   怎么区分响应咧   有一个ack类用于负责管理响应
				// 通过响应uart的响应集返回判断响应
            v->uart->clearEError();
				// 这是一个单一的航点响应
						waypointUploading();
				// 还有 时间响应 连接响应 
        }
				if(true == v->uart->getCmdFlg())
				{
					// TODO: 这里需要继续添加上位机指令集
				}
        ledHandler();
    }
}

static void waypointUploading(void)
{
	res = f_close(&fil);
	if(!res) {
		USART_Cmd(USART2,DISABLE);
		MY_DEBUG("file close succeed ! \r\n");
	} else {
		MY_DEBUG("file close come to nothing ! failure code : %d \r\n",res);
	}
	UartWayPointInit wayInit;           /* 航点初始化结构体 */
	NODE node;                          /* 链表节点结构体 */
	uint8_t j = v->uart->getSaveWaypoint();
  wayInit = v->uart->getInitWaypoint();
  //v->tools->uploadInfo(&wayInit);
  for(int i = 1; i < j; ++i)
  {
     node = *(v->myList->getNode(i));
     sendWaypoint(&node);
     delay_nms(500);
  }
  //v->missionManager->wpMission->start();
  v->myList->emptyList();
  MY_DEBUG("ok waypoint rece finish \r\n");
	renewFatfs();
	delay_nms(500);
	USART_Cmd(USART2,ENABLE);
}

static void saveAriToSd()  // 空气传感器保存函数
{
		static uint8_t i = 0;
    uint8_t count = 0;
		RTC_TimeTypeDef RTC_TimeStruct;
		RTC_DateTypeDef RTC_DateStruct;
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
		//Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type latLon = v->MySubscribe->GetGpsData();
		Ari* ariData = v->uart->getAri();
    int val = (ariData->temp - 500) / 10;
		count += sprintf(&buff[count], "20%02d-%02d-%02d ", RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date);
		count += sprintf(&buff[count], "%02d:%02d:%02d : ", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
		count += sprintf(&buff[count], "tem = %-8d hum = %-8d CO2 = %-8d VOC = %-8d PM2.5 = %-8d",
							val,(ariData->hum / 10),ariData->co2,ariData->voc,ariData->pm2_5);
		//TODO : 经纬度写入
		count += sprintf(&buff[count],"\r\n");
		i++;
    res = f_write (&fil, buff, count, &bww);
		if(!res) {
			MY_DEBUG("write file succeed , succeed number: %d! \r\n",i);
		} else {
			MY_DEBUG("write file come to nothing ! failure code : %d \r\n",res);
		}
}

static void initFatfs(void)  // 文件相关处理函数
{
		//待测试：不用指针变量
		res = f_mount(&fs, "0:", 1);
		if(!res) {
			MY_DEBUG("mount succeed ! \r\n");
		} else {
			MY_DEBUG("mount come to nothing ! \r\n");
		}
		res = f_open (&fil, fileName, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
    if(!res) {
			MY_DEBUG("file open succed.\r\n");
    } else {
       MY_DEBUG("file open not succeed, error code=%d\r\n", res);
		}
}

static void renewFatfs(void)
{
		res = f_open (&fil, fileName, FA_READ | FA_WRITE);
		if(!res) {
			MY_DEBUG("file open succed.\r\n");
    } else {
       MY_DEBUG("file open not succeed, error code=%d\r\n", res);
		}
		res = f_lseek(&fil,fil.fptr+fil.fsize);
		if(!res) {
			MY_DEBUG("lseek file succed.\r\n");
    } else {
       MY_DEBUG("lseek file not succeed, error code=%d\r\n", res);
		}
}

static void ledHandler(void) // LED相关处理函数
{
    if(!LED_RED)   // 红灯亮
    {
        LED_RED = OFF;
        LED_GREEN = OFF;
        delay_nms(200);
    }
    else
    {
        LED_GREEN = ~LED_GREEN;
        delay_nms(400);
    }
}

static bool droneInit()//飞控初始化
{
    if (!v->protocolLayer->getDriver()->getDeviceStatus())
    {
        MY_DEBUG("USART communication is not working.\r\n");
        delete (v);
        return false;
    }
    //! Initialize functional Vehicle components like
    //! Subscription, Broabcast, Control, Camera, etc
    v->functionalSetUp();
    delay_nms(500);
    // Check if the firmware version is compatible with this OSDK version
    if (v->getFwVersion() > 0 &&
            v->getFwVersion() < extendedVersionBase &&
            v->getFwVersion() != Version::M100_31)
    {
        MY_DEBUG("Upgrade firmware using Assistant software!\n");
        delete (v);
        return false;
    }
    userActivate();
    delay_nms(500);
    // Verify subscription
    if (v->getFwVersion() != Version::M100_31)
    {
        v->subscribe->verify();
        delay_nms(500);
    }
    // Obtain Control Authority
    v->obtainCtrlAuthority();
    return true;
}


