#include "basicFunction.h"
#include "rtc.h"
#include "stdio.h"

extern Vehicle        vehicle;
extern Vehicle        *v;
extern uint8_t sendBuffer[BUFF_SIZE];//DMA 发送数据缓存区

BasicFunction::BasicFunction()
#ifdef TIME
	: myTimeAndData(NULL)
#endif
{
#ifdef TIME
	myTimeAndData = new timeAndDate();
#endif
}

BasicFunction::~BasicFunction()
{
#ifdef TIME
	delete myTimeAndData;
#endif
}

//FRESULT BasicFunction::initFile()
//{
//	res = f_mount(fs, "0:", 1); 
//	if(!res)fileFlg = true;
//	return res;
//}

//FRESULT BasicFunction::readFile(const char * name, void * buffer, uint32_t len)
//{
//	if(!fileFlg)return res;
//	res = f_open (&fil, name, FA_CREATE_ALWAYS | FA_READ);
//	if(FR_OK != res)return res;
//	res = f_read(&fil,buffer,len,&bww);
//	return res;
//}
//FRESULT BasicFunction::writeFile(const char * name, void * buffer, uint32_t len)
//{
//	if(true != fileFlg)return res;
//	res = f_open (&fil, name, FA_CREATE_ALWAYS | FA_WRITE);
//	if(FR_OK != res)return res;
//	//res = f_lseek(&fil,fil.fptr + fil.fsize);
//	if(FR_OK != res)return res;
//	res = f_write (&fil, buffer, len, &bww);
//	return res;
//}
//FRESULT BasicFunction::writeFile(const char * name, timeAndDate * td, ari * ariData)
//{
//	uint8_t count = 0;
//	uint8_t buffer[150];
//	if(true != fileFlg)return res;
//	res = f_open (&fil, name, FA_CREATE_ALWAYS | FA_WRITE);
//	if(FR_OK != res)return res;
//	//res = f_lseek(&fil,fil.fptr + fil.fsize);
//	if(FR_OK != res)return res;
//	count += sprintf((char*)&buffer[count], "20%02d-%02d-%02d ", td->date->RTC_Year, td->date->RTC_Month, td->date->RTC_Date);
//  count += sprintf((char*)&buffer[count], "%02d:%02d:%02d : ", td->time->RTC_Hours, td->time->RTC_Minutes, td->time->RTC_Seconds);
//	count += sprintf((char*)&buffer[count], "tem = %-8d hum = %-8d CO2 = %-8d VOC = %-8d PM2.5 = %-8d",
//							ariData->temp,ariData->hum,ariData->co2,ariData->voc,ariData->pm2_5);
//	res = f_write (&fil, buffer, count, &bww);
//	return res;
//}

//FRESULT BasicFunction::closeFile()
//{
//	res = f_close(&fil);
//	return res;
//}

bool BasicFunction::connect()
{
	v->uart->sendCmd(DOWN_CONNECT_CMD);
	while(!v->uart->getAck()->verifyAck());  // 等待接受到响应
}

void BasicFunction::ackCmdSend(DCommand cmd,Response ack)
{
	
}

void BasicFunction::saveAckData(Response ack)
{
		
}


#ifdef MY_WAYPOINT// only for DEBUG
void BasicFunction::uploadInfo(UartWayPointInit* uartData)
{
	WayPointInitSettings fdata;
	setWaypointInitDefaults(&fdata); //初始化
	setWaypointInitParameter(uartData,&fdata); //赋值
	v->missionManager->init(WAYPOINT, 1, &fdata); //上传
}

void BasicFunction::uploadInfo(NODE* uartData)
{
	WayPointSettings wp;
	setWaypointDefaults(&wp);
	setWaypointParameter(uartData,&wp);
	uploadingWaypoint(&wp);
}

void BasicFunction::setWaypointInitDefaults(WayPointInitSettings* fdata)
{
	fdata->maxVelocity    = 10;
  fdata->idleVelocity   = 5;
  fdata->finishAction   = 0;
  fdata->executiveTimes = 1;
  fdata->yawMode        = 0;
  fdata->traceMode      = 0;
  fdata->RCLostAction   = 1;
  fdata->gimbalPitch    = 0;
  fdata->latitude       = 0;
  fdata->longitude      = 0;
  fdata->altitude       = 0;
}

void BasicFunction::setWaypointDefaults(WayPointSettings* wp)
{
  wp->damping         = 0;
  wp->yaw             = 0;
  wp->gimbalPitch     = 0;
  wp->turnMode        = 0;
  wp->hasAction       = 0;
  wp->actionTimeLimit = 100;
  wp->actionNumber    = 0;
  wp->actionRepeat    = 0;
  for (int i = 0; i < 16; ++i)
  {
    wp->commandList[i]      = 0;
    wp->commandParameter[i] = 0;
		if(i < 8)
			wp->reserved[i] = 0;
  }
}

void BasicFunction::setWaypointInitParameter(UartWayPointInit* data,WayPointInitSettings* fdata)
{
	fdata->gimbalPitch  = 1;
	fdata->idleVelocity = data->speedB + ((data->speedD + 0.1f) / (10));
	fdata->yawMode			=	data->yawMode;
	fdata->finishAction = data->endAction;
	fdata->altitude     = data->heightH << 8;
	fdata->altitude    += data->heightL;
	fdata->indexNumber  = data->wayNumber;
}

void BasicFunction::setWaypointParameter(NODE* node,WayPointSettings* wps)
{
	uint8_t test = node->id;
	wps->index        = node->id - 1;
	wps->latitude     = atof((char*)node->data->jwData.latitude) / 57.295780;
	wps->longitude    = atof((char*)node->data->jwData.longitude) / 57.295780;
	wps->altitude     = (node->data->uartWayData->heightH << 8) + node->data->uartWayData->heightL;
	wps->turnMode     = node->data->uartWayData->turnMode;
	wps->hasAction    = node->data->uartWayData->actionFlag;
	wps->actionRepeat = node->data->uartWayData->actionTime;
	wps->actionNumber = node->data->uartWayData->actionNumber;
	
	if (node->data->uartWayData->yawPM > 0x00)
	{
		wps->yaw = node->data->uartWayData->yaw;
	}
	else
	{
		wps->yaw = node->data->uartWayData->yaw * -1;
	}
	if(node->data->uartWayData->actionNumber <= 16)
	{
		for (int i = 0; i < node->data->uartWayData->actionNumber; i++)
		{
			wps->commandList[i]		= node->data->action.actionCommand[i];
			wps->commandParameter[i] = node->data->action.actionParameter[i];
			switch (wps->commandList[i])
			{
			case 0: /*< 悬停 >*/
				wps->commandParameter[i] *= 1000;
				break;
			case 5: /*< 云台 >*/
				wps->commandParameter[i] *= -1;
				break;
			case 6: /*< 飞机逆时针旋转 >*/
				wps->commandParameter[i] *= -1;
				break;
			}
		}
	}
}

void BasicFunction::uploadingWaypoint(WayPointSettings* wp)
{
	static int Num = 0;
	printf("\n\r位置(%d) %f %f %f\n\r", Num, wp->latitude * 57.295780, wp->longitude * 57.295780, wp->altitude);
	v->missionManager->wpMission->uploadIndexData(wp);
	Num++;
}
#endif

#ifdef TIME
void BasicFunction::setTimeAndDate(dateTime * dt)
{
	myTimeAndData->date->RTC_Year = dt->year;
	myTimeAndData->date->RTC_Month = dt->month;
	myTimeAndData->date->RTC_Date = dt->date;
	myTimeAndData->date->RTC_WeekDay = dt->week;
	myTimeAndData->time->RTC_Hours = dt->hour;
	myTimeAndData->time->RTC_Minutes = dt->minute;
	myTimeAndData->time->RTC_Seconds = dt->second;
	RTC_SetTime(RTC_Format_BIN,myTimeAndData->time);	
	RTC_SetDate(RTC_Format_BIN,myTimeAndData->date);
}
timeAndDate * BasicFunction::getTimeAndData()
{
	return myTimeAndData;
}
#endif
