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
	// ��ʼ����Ӧ��
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
	// ��ʼ�����ڻ�����Ϣ�ṹ��
	this->uHead = new uartBasicInfo();
	// ��ʼ����������Ϊ 0
	this->uHead->index = 0;
	// ��ʼ���������ݳ���Ϊ 0 
	this->uHead->length = 0;
	// ��ʼ�����ݻ����� Ϊ 64 �ֽ�
	this->uHead->buffer = new uint8_t[BUFF_MAX];
	// ��ʼ���Ƿ���Ҫ�������ݱ�־λΪ false
	this->uHead->savaDataFlg = false;
	// ��ʼ������ֵ Ϊ ������
	this->uHead->isCommand = UP_NONE_COMMAND;		//��ʼ��û��ָ��
	// ��ʼ����Ӧֵ Ϊ ����Ӧ
	this->uHead->isResponse = UP_NONE_ACK;			//��ʼ��û����Ӧ������Ӧ-��ʼ��״̬��
	// ��ʼ������״̬ Ϊ �մ���
	this->uHead->error = NULL_ERROR;				//��ʼ���մ��� ����ʼ��״̬��
}

//step 6
bool isCRC(uint8_t* data,uint16_t len,uint16_t crcData) // �Ա�CRC
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
	//TODO:	���isFinish = true ������ȫ����ȷ
	return isFinish;
}

//step 3
bool UartBuffer::checkDataStream()
{
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	bool isFinish = false;
	if (uHead->length < sizeof(SdataHead))	//ͷû�н������
	{
		isFinish = false;
	}
	else if (uHead->length == sizeof(SdataHead))//ͷ���������
	{
		isFinish = verifyHead();
	}
	else if (uHead->length == sHead->len)	// �������ݽ��������
	{
		isFinish = verifyData();
		if(isFinish)
		{
			if (uHead->savaDataFlg)			// �Ƿ���Ҫ������������
			{// ��Ҫ��������ʱ  ���������Լ���ջ��泤��
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
			verifyCommand();	//Ч������
			verifyResponse();	//Ч��ָ��
			prdData = (uint8_t*)sHead + sizeof(SdataHead);  // �õ��������ĵ�ַ
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
	// CRC �ɹ�
	if (!(0xBB == uHead->buffer[sHead->len - 1]))
	{
		uHead->length = 0;
		uHead->error = TRAIL_ERROR;
		return false;
	}
	// ��β �ɹ�
	if (uHead->isCommand != UP_NONE_COMMAND)executiveCommand();  // ���Ϊ�գ�ִ������
	if ((uHead->isResponse != UP_NONE_ACK) || 
		(uHead->isResponse != UP_ERROR_ACK))clearResponse();//��Ӧ��Ϊ�գ������Ӧ
	
	return true;
}

//step 4-1
void UartBuffer::verifyCommand()  // Ч������
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
void UartBuffer::verifyResponse() // Ч����Ӧ
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

void UartBuffer::executiveCommand()  //��־����״̬������ֵ
{ 
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	switch (sHead->command) 
	{
	case UP_STATUS_COMMAND:
		cmdFlg = true;
		//send("�ɻ���ʼ��״̬",sizeof("�ɻ���ʼ��״̬"));
		break;
	case UP_AIR_FEI_COMMAND:
		cmdFlg = true;
		//send("�ɻ�״̬�Ϳ���������", sizeof("�ɻ�״̬�Ϳ���������"));
		break;
	case UP_STOP_COMMAND:
		cmdFlg = true;
		//send("stop!!!", sizeof("stop!!!"));
		break;
	case UP_AIR_COMMAND:
		cmdFlg = true;
		//send("����������", sizeof("����������"));
		break;
	}
}

void UartBuffer::clearResponse() // ���һ���������Ӧ
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
	// ��ʼ�����������
	this->wayManage = new waypointManage();
	// ��ʼ������������ĺ������ֵΪ 1 --- ��������� 1 ��ʼ
	this->wayManage->countWaypoint = 1;
	// ��ʼ������������ľɺ���Ϊ 0 
	this->wayManage->saveLastWaypoint = 0;
	// ��ʼ��������
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
		uint8_t preLength = waypointSetting->data->uartWayData->longitudeLength & 0x0f; /* < �õ����ȳ��� >*/
		uint8_t dimLength = waypointSetting->data->uartWayData->latitudeLength & 0x0f;	/* < �õ�ά�ȳ��� >*/
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
		wayManage->countWaypoint++;	// ÿ�α���һ��������� - �����ֵ��һ
		if(wayManage->countWaypoint == wayManage->saveMaxWaypoint)
			initWapoint();
	}	else if ((!wayManage->isInitWaypint) && (WAYPOINT_INIT_LENGHT == uHead->length)) {
		initWaypoint = *((UartWayPointInit*)dataAddr);
		//�������󺽵���
		wayManage->saveMaxWaypoint = initWaypoint.wayNumber + 1; 
		wayManage->isInitWaypint = true;
	} 
	uHead->length = 0;                // ��ջ���������
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
	//m ɾ�����ݿ�ʼ�ĵ�ַƫ����
	//n ɾ�����ݵĸ���
	SdataHead* sHead = (SdataHead*)(uHead->buffer);
	int len = sHead->len;					//������ 10 ���ֽڣ�length�ĳ���Ϊ11
	if (p == NULL || save == NULL)return;			/*< ɾ������ͱ������鲻��Ϊ�� >*/
	if (m < 0 || n < 0)return;						/*< ƫ������ɾ����������С��0 >*/
	if (m > len || n > len)return;					/*< ƫ������ɾ�����ֶβ��ܴ����ܳ��� >*/
	if (n > (len - m))return;						/*< ɾ���ĸ������� �����ܳ���-��ַƫ���� >*/
	char* pos = p + m;								/*<	ɾ���ĵ�ַƫ����   remove address >*/
	char* cur = pos + n;							/*< ɾ���ĵ�ַ���Ǹ�Ԫ��   remove address >*/
	int i;												//ѭ������ɾ����Ԫ��
	for (i = 0; i < n; i++)
	{
		save[i] = *(pos + i);
	}
	save[i] = 0;
	//ɾ��Ԫ�� n + m = 
	for (i = 0; i < len - (n + m); i++) /*< Ҫɾ���ĸ��� remove number >*/
	{
		*(pos++) = *(cur++);
	}
}

//һ�κ�������ȫ������    ---  Ϊ�´κ���������׼��
void UartBuffer::initWapoint()
{
	wayManage->isInitWaypint = false;		// ��ʼ����־λ���
	wayManage->saveLastWaypoint = wayManage->saveMaxWaypoint; // ���汾������ĺ������
	wayManage->saveMaxWaypoint = 0;		// ��ձ��κ������
	wayManage->countWaypoint = 1;			// ��պ������ֵ
	uHead->error = RECE_OK; 					// ������ɱ�־��λ
	uHead->savaDataFlg = false;				// ��ձ������ݱ�־
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
  	USART_SendData(USARTx, buff[i]);         //�򴮿ڷ�������
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ��� 
  }
}

#ifdef MY_ARI
void UartBuffer::ariInit()
{
	// ��ʼ�������������ṹ
	pMyAri = new Ari();
	// ��ʼ������������������
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
			prdAri = (uint8_t*)ariBuff + 3; //�õ��������������ݵĵ�ַ
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