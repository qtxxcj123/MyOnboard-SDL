#ifndef _BASIC_FUN_H
#define _BASIC_FUN_H
#include "myType.h"
#include "dji_mission_type.hpp"
using namespace DJI::OSDK;

//����������
	/*
		�������ܣ�
				1������ʱ�䡣
					����ʱ����صĽṹ�����
				2����ȡʱ�䡣
					����ʱ��Ľṹ�����
				3����ȡ�ļ���
					��ȡ�ļ�������
					��ȡ���ļ�������
				4��д���ļ���
					д���������������
				5����������
	*/
//TODO : ���һ���ϴ��������ݵķ���
/*
		1����ʼ������  --  ����Ҫ�ϴ���ʼ����������
		2�������б���ĺ�������ת��Ϊ�ϴ�ʱ��Ҫ�ĺ�������
		3���ϴ���������
*/

// TODO: ��ʼ������
// TODO: ���ú������
// ------------------------------------------OK 

//TODO : ���һ�����ӹ���
/*
		�������������ڸ����ص��ã�ȷ������λ�����������Ӳ�����ִ����������
		1��������������
		2���ȴ�����ȷ�����ȴ�������Ӧ�ĵ���  --- while()
		3������true ȷ������ͨ��
*/
//-------------------------------------------OK  pass-test

//TODO : ����Ӧ�������
/*
		����������������λ����Ҫ�������ͬʱ��Ҫ��Ӧ��λ������ʱ����
		1��ȷ���������Ӧָ��
		2���������������е���Ӧֵ
*/
//TODO : �ṩ����Ӧ���͵��������
/*
		����������������λ����Ӧ��λ��ָ��ʱ��Ҫ����һЩ���� 
				���磺��Ӧ��������������ʱ���ѿ���������������������ͻ�������
*/


//TODO : ������Ӧ�������
/*
		������������������ָ�����λ��������Ӧ
*/
// ------------------ �� uart �ṩ
// -------------------------------------
//TODO : LED���
/*
		����������
			��������ִ��ʱ-�̵���˸
			���������ֹʱ-��Ƴ���
			����ӷ�����ʱ-������˸
*/
//TODO : ���Ź���� -- ���Ǽ���
/*
		�������Ź� - ���ڿ��Ź�
		ʹ�ö������Ź�---�ṩι������
*/
// -------------------------------------OK

class BasicFunction {
	
public:
	BasicFunction();
	~BasicFunction();

public:
	bool connect();  
	
	//TODO : ����Ӧ�������
/*
		����������������λ����Ҫ�������ͬʱ��Ҫ��Ӧ��λ������ʱ����
		1��ȷ���������Ӧָ��
		2���������������е���Ӧֵ
*/
//TODO : �ṩ����Ӧ���͵��������
/*
		����������������λ����Ӧ��λ��ָ��ʱ��Ҫ����һЩ���� 
				���磺��Ӧ��������������ʱ���ѿ���������������������ͻ�������
*/
	void ackCmdSend(DCommand cmd,Response ack); //���ʹ���Ӧ������
	
	void saveAckData(Response ack);             //������Ӧ�Ĳ���- ��ʱ��-������������

#ifdef MY_WAYPOINT
public://�ⲿ�ӿں���
	void uploadInfo(UartWayPointInit* uartData);
	void uploadInfo(NODE* uartData);
private://�ڲ����õĺ���
	void setWaypointInitDefaults(WayPointInitSettings* fdata);                        // ��ʼ�� WayPointInitSettings
	void setWaypointDefaults(WayPointSettings* wp);                                   // ��ʼ�� WayPointSettings
	void setWaypointInitParameter(UartWayPointInit* data,WayPointInitSettings* fdata);// �������ݱ��浽WayPointInitSettings��
	void setWaypointParameter(NODE* data,WayPointSettings* wp);                       // �������ݱ��浽WayPointSettings��
	void uploadingWaypoint(WayPointSettings* wp);								                      // �ϴ����ݵ��ɿ�
#endif

#ifdef TIME
public:
	void setTimeAndDate(dateTime* dt); // ����ʱ��
	timeAndDate* getTimeAndData();
private:
	timeAndDate* myTimeAndData;
#endif
};


#endif 
