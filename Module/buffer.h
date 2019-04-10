#ifndef _BUFFER_H
#define _BUFFER_H
#include "myType.h"
#include "ackTools.h"
//#include "listLinked.h"
#include "dma.h"
#include "list.h"

//////////////////////////////////////////////////////////////////////////
/*
��λ��ָ���
0x00����ָ��
0x01����������
0x02������ʱ������
0x03�����󺽵�����

��λ����Ӧ����
0x00������Ӧ
0x01�����طɻ���ʼ��״̬��һ�����ݣ��汾�ţ�
0x02�����طɻ�״̬�Ϳ�������������
0x03��������ͣ OK
0x04�����ؿ�������������
0xEE����Ӧ����

��λ��ָ���
0x00����ָ��
0x01������ɻ���ʼ��״̬��һ�����ݣ��汾�ţ�
0x02������ɻ�״̬�Ϳ�������������
0x03��������ͣ��������
0x04�������������������

��λ����Ӧ����
0x00������Ӧ
0x01������ȷ��
0x02������ʱ������
0x03�����غ�������
0xEE����Ӧ����
*/
//////////////////////////////////////////////////////////////////////////


//#define UP_NONE_COMMAND			0x00		/*< ��ָ��	 >*/
//#define UP_STATUS_COMMAND		0x01		/*< ״ָ̬�� >*/
//#define UP_STOP_COMMAND			0x02		/*< ��ָͣ�� >*/
//#define UP_AIR_COMMAND			0x03		/*< ����ָ�� >*/

//#define UP_OK_ACK				0x00		/*< OK��Ӧ   >*/
//#define UP_CONNECT_ACK			0x01		/*< ������Ӧ >*/
//#define UP_TIME_ACK				0x02		/*< ʱ����Ӧ >*/
//#define UP_WAYPOINT_ACK			0x03		/*< ������Ӧ >*/
//#define UP_ERROR_ACK			0x04		/*< ������Ӧ >*/

#define WAYPOINT_INIT_LENGHT 12


class UartBuffer {
public:
	UartBuffer();
	~UartBuffer();
public:
	/*
	����ԭ�ͣ�void init();
	������������
	�������أ���
	�������ã���ʼ��һЩ��Ա
	*/
	void init();
	/*
	����ԭ�ͣ�void headInit();
	������������
	�������أ���
	�������ã���ʼ��ͷָ��ĳ�Ա
	*/
	void headInit();

	/*
	����ԭ�ͣ�void wayInit();
	������������
	�������أ���
	�������ã���ʼ������ĳ�Ա
	*/
	void wayInit();

	/*
	����ԭ�ͣ�void ariInit();
	������������
	�������أ���
	�������ã���ʼ�������������ĳ�Ա
	*/
	void ariInit();
	
	/*
	����ԭ�ͣ�bool byteHandle(const uint8_t data);
	����������data�����յ��Ĵ�������
	�������أ�True | False
	����˵�������մ�������
	*/
	bool byteHandler(const uint8_t data);	//step 1

	/*
	����ԭ�ͣ�void storeDataBuffer(const uint8_t data);
	����������data����Ҫ���������
	�������أ���
	����˵�����������ݵ�������
	*/
	bool storeDataBuffer(const uint8_t data);	//step 2

	/*
	����ԭ�ͣ�bool checkDataStream();
	������������
	�������أ�True | False
	����˵�������������
	*/

	bool checkDataStream();				//step 3

	/*
	����ԭ�ͣ�void isCRC(uint8_t* data,uint8_t len);
	����������data����ҪЧ�������	len�����ݳ���
	�������أ�True | False
	����˵����Ч��CRC
	*/
	//bool isCRC(uint8_t* data,uint8_t len);

	/*
	����ԭ�ͣ�bool verifyHead();
	������������
	�������أ�True | False
	����˵������֤ͷ
	*/
	bool verifyHead();
	
	/*
	����ԭ�ͣ�bool verifyData();
	������������
	�������أ�True | False
	����˵������֤����
	*/
  
	bool verifyData();

	/*
	����ԭ�ͣ�void verifyCommand();
	������������
	�������أ���
	����˵������֤����
	*/
	void verifyCommand();

	/*
	����ԭ�ͣ�void verifyResponse();
	������������
	�������أ���
	����˵������֤��Ӧ
	*/
	void verifyResponse();

	/*
	����ԭ�ͣ�void executiveCommand();
	������������
	�������أ���
	����˵����ִ��ָ��
	*/
	void executiveCommand();

	/*
	����ԭ�ͣ�void clearResponse();
	������������
	�������أ���
	����˵�������һ����Ӧ
	*/
	void clearResponse();

	/*
	����ԭ�ͣ�void saveTimeData(void* source);
	����������dataAddr��Դ���ݵ�ַ
	�������أ���
	����˵��������ʱ������
	*/
	void saveTimeData(void* dataAddr);
#ifdef MY_WAYPOINT	
	/*
	����ԭ�ͣ�void saveTimeData(void* source);
	����������dataAddr��Դ���ݵ�ַ
	�������أ���
	����˵�������溽������
	*/
	void saveWaypoint(void* dataAddr);

	/*
	����ԭ�ͣ�void remove_nm(char * p, int m, int n, uint8_t * save);
	����������p��ԭ��ַ  m��ɾ���׵�ַƫ����   n��ɾ������   save��ɾ�����ݱ����ַ
	�������أ���
	�������ܣ�ɾ�������е����ݱ��浽��һ��������
	*/
	void remove_nm(char * p, int m, int n, uint8_t * save);

	/*
	����ԭ�ͣ�initWapoint
	������������
	�������أ���
	�������ܣ�һ�κ�������ȫ����������չ�����
	*/
	void initWapoint();
	
	/*
	����ԭ�ͣ�waypointManage* getWayManage()
	������������
	�������أ�uint8_t
	����˵�������غ�����Ŀ
	*/
	// �ṩ����纽��Ĺ����������ڲ鿴������ܵ�״̬
	uint8_t getSaveWaypoint();
	
	UartWayPointInit getInitWaypoint();
#endif
	// --------------------------------------------------------
	// �������
	/*
	����ԭ�ͣ�void send(USART_TypeDef* USARTx,void* data,uint8_t len);
	����������USARTx�����͵��Ǹ����� data�������͵�����	len�����ݳ���
	�������أ���
	����˵�����������ݵ�����
	*/
	void send(USART_TypeDef* USARTx,const void* data, uint8_t len);

	/*
	����ԭ�ͣ�void sendCmd(Command cmd);
	����������cmd��ָ��ö��
	�������أ���
	����˵��������ָ��
	*/
	// ����һ��ָ������һ����Ӧ���ȴ�������Ӧ�����
	void sendCmd(DCommand cmd);
	
	/*
	����ԭ�ͣ�void sendDma(void* data,uint8_t len)
	����������data������������	len�����ͳ���
	�������أ���
	����˵�����������ݵ�DMA
	*/
	void sendDma(void* data,uint8_t len);

	/*
	����ԭ�ͣ�void setPutUp(Command cmd);
	����������cmd��������Ӧ������
	�������أ���
	����˵��������һ����Ӧ
	*/
	void setPutUp(DCommand cmd);
  
#ifdef MY_ARI
  /*
	����ԭ�ͣ�void receAir(const uint8_t data);
	����������data����������������
	�������أ�true | false
	����˵�������ܿ�������������
	*/
  bool receAir(const uint8_t data);
    
  /*
	����ԭ�ͣ�void ariHandler();
	������������
	�������أ�true | false
	����˵���������������������
	*/
  bool ariHandler();
	
  /*
	����ԭ�ͣ�void verifyAriHead();
	������������
	�������أ���
	����˵����Ч���������������ͷ
	*/
	void verifyAriHead();
	
  /*
	����ԭ�ͣ�void verifyAri();
	������������
	�������أ�true | false
	����˵����Ч�����������������
	*/
  bool verifyAri();
	
  /*
	����ԭ�ͣ�ari getAri();
	������������
	�������أ�ari
	����˵�������ؿ�������������
	*/
	Ari* getAri();
	
	/*
	����ԭ�ͣ�saveAriData(uint8_t* prdAri)
	����������prdAri
	�������أ���
	����˵���������������������
	*/
	void saveAriData(uint8_t* prdAri);
	
	/*
	����ԭ�ͣ�stratAri()
	������������
	�������أ���
	����˵�������������������ϴ����޷��رգ�
	*/
	void startAri();
	
	/*
		��ȡһЩ��Ա�ĺ���������һЩ��Ա�ĺ���
	*/
#endif
	
	/*
	����ԭ�ͣ�getCmdFlg
	������������
	�������أ�true | false
	����˵�������ظ�����Ƿ�����Ҫִ�е�����
	*/
	bool getCmdFlg();		// �ṩ�������ѯ����
	/*
	����ԭ�ͣ�setCmdFlg(bool b)
	����������b�����
	�������أ�true | false
	����˵�������ظ�����Ƿ�����Ҫִ�е�����
	*/
	void clearCmdFlg();	// �ṩ�������������־
	/*
	����ԭ�ͣ�getCommand()
	������������
	�������أ�Ecommand
	����˵�������ظ����һ��ȷ����ָ���������Ҫִ������ָ�
	*/
	Ecommand getCommand();	// �ṩ�����ȷ��������
	/*
	����ԭ�ͣ�getEError()
	������������
	�������أ�EError
	����˵�������ظ�������״̬������������ɣ�
	*/
	EError getEError();   // �ṩ����error״̬
	/*
	����ԭ�ͣ�clearEError()
	������������
	�������أ���
	����˵�������ظ������մ���Ľӿ�
	*/
	void clearEError();
	
	/*
	����ԭ�ͣ�getAck()
	������������
	�������أ�ackTools*
	����˵�������ظ������Ӧ�๤��
	*/
	ackTools* getAck();

private:  // ��ͷ��صĳ�Ա
	uartBasicInfo* uHead;		/*< ���ڻ�����Ϣ-����һЩ�ر���Ϣ[���ݵĸ������� - ��֡���ݵĳ��� - ���ݻ����� - ��]>*/
	uint8_t* prdData;				/*< ָ����������ָ�� >*/			
	ackTools* ack;					/*< ackӦ�����ָ��  ����ȷ����Ӧ�ȵ���� >*/
	bool cmdFlg;						/*< �ṩ������ж��Ƿ�����Ҫִ������ı�־λ >*/
	const uint8_t BUFF_MAX; /*< �����������ֵ >*/
#ifdef MY_WAYPOINT
private://������س�Ա
	waypointManage* wayManage;		  /*< ��������� ������Ļ�����Ϣ - ���������>*/
	UartWayPointInit initWaypoint; /*< ���ڽ��պ����ʼ����ָ�� >*/
	NODE*	waypointSetting;					/*< ��������  ͨ���ó�Ա�Ѻ���������ӵ����� >*/
#endif
#ifdef TIME
private://ʱ����س�Ա
	dateTime* date;			/* ���ڱ���ʱ���ָ�� */
#endif


#ifdef MY_ARI
	Ari* pMyAri;						/*< �����������������ָ�� >*/
	uint8_t* ariBuff;				/*< �������������ݻ����� >*/	
	uint8_t* prdAri;				/*< ָ��������������ݵ�ָ�� >*/
	uint8_t ariCount;				/*< ��������������ֵ >*/
	const uint8_t ARI_MAX;	/*< �����������������ֵ >*/
#endif
};


#endif
