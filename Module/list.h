#ifndef _LIST_H
#define _LIST_H
#include "myType.h"

class list {
public:
	list();
	~list();
public:
	// ��ʼ������
	void initList();
	// ��ӽڵ�
	void addNode(NODE* newNode);
	// ���ؽڵ�
	NODE* getNode(uint8_t id);
	// ɾ���ڵ�
	void removeNode(uint8_t id);
	// �������
	void emptyList();
private:
	NODE* myHead;
};



#endif

