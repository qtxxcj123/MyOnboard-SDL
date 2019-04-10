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
//������صĽṹ


typedef enum Ecommand { //��λ�����յ���ָ������
	UP_NONE_COMMAND = 0x00,				/*< ��λ�����������>*/
	UP_STATUS_COMMAND = 0x01,			/*< ��λ���������״̬����>*/
	UP_AIR_FEI_COMMAND = 0x02,		/*< ��λ�������������������ͷɻ�״̬����>*/
	UP_STOP_COMMAND = 0x03,				/*< ��λ���������ֹͣ����>*/
	UP_AIR_COMMAND = 0x04					/*< ��λ����������������������>*/
}Ecommand;

typedef enum Eresponse {//��λ�����յ�����Ӧ����
	UP_NONE_ACK = 0x00,						/*< ��λ����Ӧ������Ӧ - ��Ӧ��λ��������>*/
	UP_CONNECT_ACK = 0x01,				/*< ��λ����Ӧ��������Ӧ - ��Ӧ��λ����������>*/
	UP_TIME_ACK = 0x02,						/*< ��λ����Ӧ��ʱ����Ӧ - ��Ӧ��λ������ʱ������>*/
	UP_WAYPOINT_ACK = 0x03,				/*< ��λ����Ӧ��������Ӧ - ��Ӧ��λ�����󺽵�����>*/
	UP_ERROR_ACK = 0xEE						/*< ��λ����Ӧ��������Ӧ - ��Ӧ��λ�����ʹ�������>*/
}Eresponse;

typedef enum EError {//��������
	RECE_OK = 0,		/*< ���ճɹ�����ʾ������� >*/
//	NONE,						/*< û��״̬����ʾû���κ���� >*/
	HEAD_ERROR,			/*< ͷ���󣺱�ʾ���յ��˴����֡ͷ >*/												
	TRAIL_ERROR,		/*< β���󣺱�ʾ���յ��˴����֡β >*/		
	CRC_ERROR,			/*< CRC���󣺱�ʾCRCУ����ִ��� >*/	
	NULL_ERROR,			/*< �մ��󣺱�ʾ������״̬ - ��ʼʱ��״̬ >*/	
	COMMAND_ERROR,	/*< ���Ӵ��󣺱�ʾ���ӳ����˴��� >*/	
	RESPONSE_ERROR	/*< ��Ӧ���󣺱�ʾ��������Ӧ���� >*/	
}EError;

typedef struct uartBasicInfo {
	uint8_t index;			//���ݵ����������յ���֡����+1			
	uint16_t length;		//���ݵĳ��ȣ����յ�����+1
	uint8_t* buffer;		//���ݻ�����		
	bool savaDataFlg;		//�Ƿ���Ҫ��������
	Ecommand isCommand;		//ָ�	
	Eresponse isResponse;   //��Ӧ�� 
	EError error;			//��������
}uartBasicInfo;

typedef struct SdataHead {
	uint8_t head : 8;		/*< ��ͷ	 >*/
	uint8_t len : 8;		/*< ���ݳ��� >*/
	uint8_t command : 8;	/*< ���	 >*/
	uint8_t response : 8;	/*< ��Ӧ��   >*/
}SdataHead;


typedef struct UartWayPointInit {		  // �����ʼ�����սṹ��
	uint8_t speedB : 4;					  // ��ʼ���ٶ�
	uint8_t speedD : 4;					  // ��ʼ���ٶ�
	uint8_t yawMode : 4;				  // ƫ��ģʽ
	uint8_t endAction : 4;				  // ��������
	uint8_t heightH : 8;				  // �߶ȣ���λ��
	uint8_t heightL : 8;				  // �߶ȣ���λ��
	uint8_t wayNumber : 8;				  // ��������	
}UartWayPointInit;

typedef struct UartWayPointData {			//����������սṹ��
	uint8_t latitudeLength : 4;				// ά�ȳ���
	uint8_t longitudeLength : 4;			// ���ȳ���
	uint8_t heightH : 8;				    // �߶ȸ�
	uint8_t heightL : 8;				    // �߶ȵ�
	uint8_t turnMode : 4;				    // ת��ģʽ
	uint8_t yawPM : 4;						// ƫ������
	uint8_t yaw : 8;						// ƫ����
	uint8_t actionFlag : 4;					// ������־
	uint8_t actionTime : 4;					// ��������
	uint8_t actionNumber : 8;				// ��������
}UartWayPointData;

typedef struct dateTime {
	uint8_t year;        //12
	uint8_t month;        //��
	uint8_t date;          //��
	uint8_t week;		  //����
	uint8_t hour;         //ʱ
	uint8_t minute;       //��
	uint8_t second;       //��
}dateTime;

typedef struct PreDim {
	uint8_t longitude[11];				// ����
	uint8_t latitude[10];				  // ά��
}PreDim;

typedef struct Actions {
	uint8_t actionCommand[16];			  // ��������
	uint8_t actionParameter[16];			// ��������
}Actions;

typedef struct waypoint {
	UartWayPointData* uartWayData;//�ڵ�����
	PreDim		jwData;	//��γ������
	Actions		action;	//��������
}waypoint;
// TODO ��Ϊָ��

typedef struct NODE {	//�ڵ�
	uint8_t id;			//�ڵ�id
	waypoint* data;		//������������
	NODE* next;			//�¸��ڵ�
}NODE;

typedef struct waypointManage {
	bool isInitWaypint; //���������Ƿ񱻳�ʼ����
	uint8_t countWaypoint; //��¼����ĺ������
	uint8_t saveMaxWaypoint; //��¼���ĺ������
	uint8_t saveLastWaypoint; //��¼�ϴκ���ĸ���
}waypointManage;


typedef struct Ari{
   uint16_t co2 : 16;
   uint16_t voc : 16;
   uint16_t hum : 16;
   uint16_t temp : 16;
   uint16_t pm2_5 : 16;
}Ari;



//---------------------------------------------------------------------------------------
//������صĽṹ

typedef struct SendBufferOne {                                           /*< һ������ >*/
	//uint8_t                                                  head = 0xFD;	 /*< ��ͷ >*/		
	// Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type     latLon;       /*< GPS >*/
	// char*                                                    SerialNum;    /*< �汾�� >*/ 
}SendBufferOne;


typedef struct SendBuffer {
	//uint8_t                                                     head = 0xFD;  /*< ��ͷ >*/		
	// Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type        latLon;       /*< GPS  >*/
	// Telemetry::TypeMap<Telemetry::TOPIC_BATTERY_INFO>::type     battery;      /*< ��� >*/
	// Telemetry::TypeMap<Telemetry::TOPIC_GIMBAL_ANGLES>::type    gimbal;       /*< ��̨ >*/
	// Telemetry::TypeMap<Telemetry::TOPIC_STATUS_FLIGHT>::type    flight;       /*< �ɻ� >*/
}SendBuffer;

typedef struct sendCommand { 
	uint8_t* noneCmd;		//��ָ��
	uint8_t* connectCmd;	//����/�ж�ָ��
	uint8_t* timeCmd;		//ʱ��ָ��
	uint8_t* waypointCmd;	//����ָ��
}sendCommand;

typedef enum DCommand { //��λ�����͵�ָ������
	DOWN_NONE_CMD = 0x00,				/*< ��λ����������� >*/
	DOWN_CONNECT_CMD = 0x01,		/*< ��λ����������������� >*/
	DOWN_TIME_CMD = 0x02,				/*< ��λ���������ʱ������ >*/
	DOWN_WAYPOINT_CMD = 0x03		/*< ��λ��������󺽵����� >*/
}DCommand;

typedef enum Response { //��λ�����͵���Ӧ����
	DOWN_NONE_ACK = 0x00,				/*< ��λ����Ӧ������Ӧ - ��Ӧ��λ�������� >*/
	DOWN_INIT_ACK = 0x01,				/*< ��λ����Ӧ����ʼ����Ӧ - ��Ӧ��λ������ɿس�ʼ��״̬���� >*/
	DOWN_ARI_FEI_ACK = 0x02,		/*< ��λ����Ӧ�������������ͷɻ���Ӧ -��Ӧ��λ���ɻ�״̬�Ϳ����������������� >*/
	DOWN_STOP_ACK = 0x03,				/*< ��λ����Ӧ����ͣ��Ӧ - ��Ӧ��λ��������ͣ�����㣩���� >*/
	DOWN_ARI_ACK = 0x04,				/*< ��λ����Ӧ��������������Ӧ - ��Ӧ��λ���������������� >*/
	DOWN_ERROR = 0xEE						/*< ��λ����Ӧ��������Ӧ - ��Ӧ��λ�������˴����������Ӧ >*/
}Response;


const uint8_t noneCmd[ARRAY_SIZE] =		{ 0xFF,0x0A,0x00,0x00,0x43,0x4E,0x44,0xE2,0xD0,0xBB };   	/*< ��λ��������̶����ݸ�ʽ >*/
const uint8_t connectCmd[ARRAY_SIZE] =	{ 0xFF,0x0A,0x01,0x00,0x44,0x4E,0x44,0xE3,0x5C,0xBB }; 	/*< ��λ���������ӹ̶����ݸ�ʽ >*/
const uint8_t timeCmd[ARRAY_SIZE] =		{ 0xFF,0x0A,0x02,0x00,0x45,0x4E,0x44,0x23,0x49,0xBB };		/*< ��λ������������������ݹ̶����ݸ�ʽ >*/
const uint8_t waypointCmd[ARRAY_SIZE] = { 0xFF,0x0A,0x03,0x00,0x46,0x4E,0x44,0xE3,0x84,0xBB };	/*< ��λ�����󺽵�����̶���ʽ >*/



const uint8_t startAriArray[] = {0x11, 0x02, 0x01, 0x00, 0xEC};		/*< ����ʼ�������������ݻش��̶����ݸ�ʽ >*/

//�������������Ľṹ�嶨��
//---��������ʱ������ṹ
typedef struct timeAndDate {
	RTC_TimeTypeDef* time;	/*< RTCʱ��ṹ�� >*/
	RTC_DateTypeDef* date;  /*< RTC���ڽṹ�� >*/
}timeAndDate;

//}
#endif
