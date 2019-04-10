#include "dji_vehicle.hpp"
extern uint8_t sendBuffer[BUFF_SIZE];
extern Vehicle        vehicle ;
extern Vehicle        *v;
#ifdef TEST_TOOL
extern list* myList;
#endif
UartBuffer::UartBuffer() :
	uHead(NULL)
	, BUFF_MAX(64)
	, prdData(NULL)
	, ack(NULL)
	, cmdFlg(false)
#ifdef MY_WAYPOINT
	//,initWaypoint(NULL)
	,waypointSetting(NULL)
	,wayManage(NULL)
#endif
#ifdef TIME
	,date(NULL)
#endif
#ifdef MY_ARI
	, ARI_MAX(14)
	, ariBuff(NULL)
	, prdAri(NULL)
	, pMyAri(NULL)
	, ariCount(0)
#endif
{
	init();
}

UartBuffer::~UartBuffer()
{
	delete[] uHead->buffer;
	delete uHead;
#ifdef MY_WAYPOINT
	delete wayManage;
#endif
#ifdef TIME
	delete date;
#endif
#ifdef MY_ARI
	delete[] ariBuff;
	delete pMyAri;
#endif
}

void UartBuffer::init()
{
	// 初始化响应类
	ack = new ackTools();
	headInit();
#ifdef MY_WAYPOINT
	wayInit();
#endif
#ifdef TIME
	this->date = new dateTime();
#endif
#ifdef MY_ARI
  ariInit();
#endif
}

void UartBuffer::headInit()
{
	// 初始化串口基本信息结构体
	this->uHead = new uartBasicInfo();
	// 初始化基本索引为 0
	this->uHead->index = 0;
	// 初始化接收数据长度为 0 
	this->uHead->length = 0;
	// 初始化数据缓存区 为 64 字节
	this->uHead->buffer = new uint8_t[BUFF_MAX];
	// 初始化是否需要保存数据标志位为 false
	this->uHead->savaDataFlg = false;
	// 初始化命令值 为 空命令
	this->uHead->isCommand = UP_NONE_COMMAND;		//初始化没有指令
	// 初始化响应值 为 空响应
	this->uHead->isResponse = UP_NONE_ACK;			//初始化没有响应（空响应-初始化状态）
	// 初始化错误状态 为 空错误
	this->uHead->error = NULL_ERROR;				//初始化空错误 （初始化状态）
}

//step 6
bool isCRC(uint8_t* data,uint16_t len,uint16_t crcData) // 对比CRC
{
	uint64_t crc = getCRC(data, len - 3);
	if (crcData == crc)return true;
	return false;
}

//step 1 
bool UartBuffer::byteHandler(const uint8_t data)
{
	storeDataBuffer(data);
	return false;
}

//step 2
bool UartBuffer::storeDataBuffer(const uint8_t data)
{
	bool isFinish = false;
	if (uHead->length != BUFF_MAX)
	{
		uHead->buffer[uHead->length] = data;
		uHead->length++;
	}
	else
	{
		memset(uHead->buffer, 0, uHead->length);
		uHead->length = 0;
	}
	isFinish = checkDataStream();
	//TODO:	如果isFinish = true 则数据全部正确
	return isFinish;
}

//step 3
bool UartBuffer::checkDataStream()
{
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	bool isFinish = false;
	if (uHead->length < sizeof(SdataHead))	//头没有接受完成
	{
		isFinish = false;
	}
	else if (uHead->length == sizeof(SdataHead))//头接受完成了
	{
		isFinish = verifyHead();
	}
	else if (uHead->length == sHead->len)	// 所有数据接收完成了
	{
		isFinish = verifyData();
		if(isFinish)
		{
			if (uHead->savaDataFlg)			// 是否需要启动保存数据
			{// 需要保存数据时  保存数据自己清空缓存长度
#ifdef MY_WAYPOINT
				if(UP_WAYPOINT_ACK == uHead->isResponse)
					saveWaypoint(prdData);
#endif
#ifdef TIME
				else if(UP_TIME_ACK == uHead->isResponse)
					saveTimeData(prdData);
#endif	
			} else {
				uHead->length = 0;
				uHead->error = RECE_OK;
			}
		}
	}
	return isFinish;
}

//step 4
bool UartBuffer::verifyHead()
{
	bool isFinish = false;
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	if (0xFC == sHead->head)
	{
		if (uHead->length == sizeof(SdataHead))
		{
			verifyCommand();	//效验命令
			verifyResponse();	//效验指令
			prdData = (uint8_t*)sHead + sizeof(SdataHead);  // 得到数据区的地址
		}
	}
	else
	{
		uHead->length = 0;
		uHead->error = HEAD_ERROR;
		isFinish = false;
	}
	return isFinish;
}

//step 5
bool UartBuffer::verifyData()
{
	uint16_t crcBuf = 0;
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	crcBuf = uHead->buffer[sHead->len - 3] * 256;
	crcBuf += uHead->buffer[sHead->len - 2];
	if (!isCRC(uHead->buffer, sHead->len, crcBuf))
	{
		uHead->length = 0;
		uHead->error = CRC_ERROR;
		return false;
	}
	// CRC 成功
	if (!(0xBB == uHead->buffer[sHead->len - 1]))
	{
		uHead->length = 0;
		uHead->error = TRAIL_ERROR;
		return false;
	}
	// 包尾 成功
	if (uHead->isCommand != UP_NONE_COMMAND)executiveCommand();  // 命令不为空，执行命令
	if ((uHead->isResponse != UP_NONE_ACK) || 
		(uHead->isResponse != UP_ERROR_ACK))clearResponse();//响应不为空，清空响应
	
	return true;
}

//step 4-1
void UartBuffer::verifyCommand()  // 效验命令
{
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	switch (sHead->command)
	{
	case UP_NONE_COMMAND:
		uHead->isCommand = UP_NONE_COMMAND;
		break;
	case UP_STATUS_COMMAND:
		uHead->isCommand = UP_STATUS_COMMAND;
		break;
	case UP_AIR_FEI_COMMAND:
		uHead->isCommand = UP_AIR_FEI_COMMAND;
		break;
	case UP_STOP_COMMAND:
		uHead->isCommand = UP_STOP_COMMAND;
		break;
	case UP_AIR_COMMAND:
		uHead->isCommand = UP_AIR_COMMAND;
		break;
	default:
	{
		uHead->isCommand = UP_NONE_COMMAND;
		uHead->error = COMMAND_ERROR;
	}	
	}
}

//step 4-2
void UartBuffer::verifyResponse() // 效验响应
{
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	switch (sHead->response)
	{
	case UP_NONE_ACK:
		uHead->isResponse = UP_NONE_ACK;
		break;
	case UP_CONNECT_ACK:
		uHead->isResponse = UP_CONNECT_ACK;
		break;
	case UP_TIME_ACK:
		uHead->isResponse = UP_TIME_ACK;
		break;
	case UP_WAYPOINT_ACK:
		uHead->isResponse = UP_WAYPOINT_ACK;
		break;
	case UP_ERROR_ACK:
		uHead->isResponse = UP_ERROR_ACK;
		break;
	default:
	{
		uHead->isResponse = UP_NONE_ACK;
		uHead->error = RESPONSE_ERROR;
	}
	}
}

void UartBuffer::executiveCommand()  //标志命令状态和命令值
{ 
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	switch (sHead->command) 
	{
	case UP_STATUS_COMMAND:
		cmdFlg = true;
		//send("飞机初始化状态",sizeof("飞机初始化状态"));
		break;
	case UP_AIR_FEI_COMMAND:
		cmdFlg = true;
		//send("飞机状态和空气传感器", sizeof("飞机状态和空气传感器"));
		break;
	case UP_STOP_COMMAND:
		cmdFlg = true;
		//send("stop!!!", sizeof("stop!!!"));
		break;
	case UP_AIR_COMMAND:
		cmdFlg = true;
		//send("空气传感器", sizeof("空气传感器"));
		break;
	}
}

void UartBuffer::clearResponse() // 清空一个挂起的响应
{
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	switch (sHead->response)
	{
	case UP_CONNECT_ACK:
		ack->setAckRep(UP_CONNECT_ACK);   
		break;
	case UP_TIME_ACK:
		ack->setAckRep(UP_TIME_ACK); 
		uHead->savaDataFlg = true;
		break;
	case UP_WAYPOINT_ACK:
		ack->setAckRep(UP_WAYPOINT_ACK);
		uHead->savaDataFlg = true;
		break;
	}
}
#ifdef TIME
void UartBuffer::saveTimeData(void * dataAddr)
{

}
#endif
#ifdef MY_WAYPOINT
void UartBuffer::wayInit()
{
	// 初始化航点管理器
	this->wayManage = new waypointManage();
	// 初始化航点管理器的航点计数值为 1 --- 航点计数从 1 开始
	this->wayManage->countWaypoint = 1;
	// 初始化航点管理器的旧航点为 0 
	this->wayManage->saveLastWaypoint = 0;
	// 初始化链表类
}

void UartBuffer::saveWaypoint(void * dataAddr)
{
	if( true == wayManage->isInitWaypint){
		this->waypointSetting = new NODE();
		this->waypointSetting->data = new waypoint();
		//this->waypointSetting = (NODE*)mymalloc(SRAMIN,sizeof(NODE));
		//this->waypointSetting->data = (waypoint*)mymalloc(SRAMIN,sizeof(waypoint));
		
//		this->waypointSetting = (NODE*)v->mmu->mymalloc(sizeof(NODE));
//		this->waypointSetting->data = (waypoint*)v->mmu->mymalloc(sizeof(waypoint));
		waypointSetting->id = wayManage->countWaypoint;
		waypointSetting->data->uartWayData = (UartWayPointData*)dataAddr;
		uint8_t preLength = waypointSetting->data->uartWayData->longitudeLength & 0x0f; /* < 得到经度长度 >*/
		uint8_t dimLength = waypointSetting->data->uartWayData->latitudeLength & 0x0f;	/* < 得到维度长度 >*/
		remove_nm((char*)dataAddr, 1, dimLength, waypointSetting->data->jwData.latitude);
		remove_nm((char*)dataAddr, 1, preLength, waypointSetting->data->jwData.longitude);
		remove_nm((char*)dataAddr, 7, waypointSetting->data->uartWayData->actionNumber,
			waypointSetting->data->action.actionCommand);
		remove_nm((char*)dataAddr, 7, waypointSetting->data->uartWayData->actionNumber, 
			waypointSetting->data->action.actionParameter);																											 
#ifdef  TEST_TOOL
		myList->addNode(waypointSetting);
#else
		v->myList->addNode(waypointSetting);
#endif
		wayManage->countWaypoint++;	// 每次保存一个航点个数 - 则计数值加一
		if(wayManage->countWaypoint == wayManage->saveMaxWaypoint)
			initWapoint();
	}	else if ((!wayManage->isInitWaypint) && (WAYPOINT_INIT_LENGHT == uHead->length)) {
		initWaypoint = *((UartWayPointInit*)dataAddr);
		//保存的最大航点数
		wayManage->saveMaxWaypoint = initWaypoint.wayNumber + 1; 
		wayManage->isInitWaypint = true;
	} 
	uHead->length = 0;                // 清空缓存区长度
}

uint8_t UartBuffer::getSaveWaypoint()
{
	return wayManage->saveLastWaypoint;
}

UartWayPointInit UartBuffer::getInitWaypoint()
{
	return this->initWaypoint;
}
void UartBuffer::remove_nm(char * p, int m, int n, uint8_t * save)
{
	//m 删除数据开始的地址偏移量
	//n 删除数据的个数
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	int len = sHead->len;					//接收了 10 个字节，length的长度为11
	if (p == NULL || save == NULL)return;			/*< 删除数组和保存数组不能为空 >*/
	if (m < 0 || n < 0)return;						/*< 偏移量和删除个数不能小于0 >*/
	if (m > len || n > len)return;					/*< 偏移量和删除总字段不能大于总长度 >*/
	if (n > (len - m))return;						/*< 删除的个数不能 大于总长度-地址偏移量 >*/
	char* pos = p + m;								/*<	删除的地址偏移量   remove address >*/
	char* cur = pos + n;							/*< 删除的地址到那个元素   remove address >*/
	int i;												//循环保存删除的元素
	for (i = 0; i < n; i++)
	{
		save[i] = *(pos + i);
	}
	save[i] = 0;
	//删除元素 n + m = 
	for (i = 0; i < len - (n + m); i++) /*< 要删除的个数 remove number >*/
	{
		*(pos++) = *(cur++);
	}
}

//一次航点任务全部结束    ---  为下次航点任务做准备
void UartBuffer::initWapoint()
{
	wayManage->isInitWaypint = false;		// 初始化标志位清空
	wayManage->saveLastWaypoint = wayManage->saveMaxWaypoint; // 保存本次任务的航点个数
	wayManage->saveMaxWaypoint = 0;		// 清空本次航点个数
	wayManage->countWaypoint = 1;			// 清空航点计数值
	uHead->error = RECE_OK; 					// 接收完成标志置位
	uHead->savaDataFlg = false;				// 清空保存数据标志
	uHead->isResponse = UP_NONE_ACK;
}
#endif
//--------------------------------------------------------send_data---------------
void UartBuffer::sendCmd(DCommand cmd)
{
	switch (cmd)
	{
	case DOWN_NONE_CMD: 
		{
			sendDma((void*)noneCmd,ARRAY_SIZE);
			setPutUp(DOWN_NONE_CMD);
			break;
		}
	case DOWN_CONNECT_CMD:
		{
			sendDma((void*)connectCmd,ARRAY_SIZE);
			setPutUp(DOWN_CONNECT_CMD);
			break;
		}
	case DOWN_TIME_CMD:
		{
			sendDma((void*)timeCmd,ARRAY_SIZE);
			setPutUp(DOWN_TIME_CMD);
			break;
		}
	case DOWN_WAYPOINT_CMD:
		{
			sendDma((void*)waypointCmd,ARRAY_SIZE);
			setPutUp(DOWN_WAYPOINT_CMD);
			break;
		}
	}
}

void UartBuffer::sendDma(void* data,uint8_t len)
{
	uint8_t* buff = (uint8_t*)data;
	for(int i = 0; i < len; ++i)
	{
		sendBuffer[i] = buff[i];
	}
	DMA_Start(len);
}

void UartBuffer::send(USART_TypeDef* USARTx,const void * data, uint8_t len)
{
  const uint8_t* buff = (const uint8_t*)data;
	uint16_t count = 1000;
  for(int i = 0; i < len; i++)
  {
		while(count--);
  	USART_SendData(USARTx, buff[i]);         //向串口发送数据
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束 
  }
}

#ifdef MY_ARI
void UartBuffer::ariInit()
{
	// 初始化空气传感器结构
	pMyAri = new Ari();
	// 初始化空气传感器缓存区
	ariBuff = new uint8_t[ARI_MAX];
}

// ari setp 1 
bool UartBuffer::receAir(const uint8_t data)
{
	bool isFinish = false;
	if (ariCount != ARI_MAX)
	{
		ariBuff[ariCount] = data;
		ariCount++;
	}
	else
	{
		memset(ariBuff, 0, ariCount);
		ariCount = 0;
	}
	isFinish = ariHandler();
	return isFinish;
}

// ari setp 2
bool UartBuffer::ariHandler()
{
	bool isFinish = false;
	if(3 == ariCount)
	{
		verifyAriHead();
	}
	if(ARI_MAX == ariCount)
	{
		if(verifyAri()) 
		{
			prdAri = (uint8_t*)ariBuff + 3; //得到空气传感器数据的地址
			saveAriData(prdAri);
			isFinish = true;
		}
	}
	return isFinish;
}

//ari setp 3
void UartBuffer::verifyAriHead()
{
	if (ariBuff[0] == 0x16) {
		if (ariBuff[1] == 0x0B) {
			if (ariBuff[2] == 0x01) {
			} else {
				ariCount = 0;
				return;
			}
		} else {
			ariCount = 0;
			return;
		}
	} else {
		ariCount = 0;
		return;
	}
}

// ari setp 4
bool UartBuffer::verifyAri()
{
	uint8_t sum = 0;
	bool isFinish = false;
	ariCount = 0;
	for(int i = 0; i < ARI_MAX - 1; i++)
	{
		sum += ariBuff[i];
	}
	sum = ~sum + 1;
	if(sum == ariBuff[ARI_MAX - 1])return true;
	else return false;
}	
	
// ari setp 5
Ari* UartBuffer::getAri()
{
	return this->pMyAri;
}

void UartBuffer::saveAriData(uint8_t* prdAri)
{
	uint16_t temp = 0;
	uint16_t* p = (uint16_t*)pMyAri;
	for (int i = 0; i < 5; i++)
	{
		temp = *prdAri++ * 256;
		temp += *prdAri++;
		*p++ = temp;
	}
}

void UartBuffer::startAri()
{
	send(USART2,startAriArray,5);
}
#endif

void UartBuffer::setPutUp(DCommand cmd)
{
	this->ack->setAckCmd(cmd);
}

bool UartBuffer::getCmdFlg()
{
	return this->cmdFlg;
}

void UartBuffer::clearCmdFlg()
{
	this->cmdFlg = false;
}
Ecommand UartBuffer::getCommand()
{
	return this->uHead->isCommand;
}

EError UartBuffer::getEError()
{
	return this->uHead->error;
}

void UartBuffer::clearEError()
{
	this->uHead->error = NULL_ERROR;
}

ackTools* UartBuffer::getAck()
{
	return this->ack;
}