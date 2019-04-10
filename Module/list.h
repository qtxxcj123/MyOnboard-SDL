#ifndef _LIST_H
#define _LIST_H
#include "myType.h"

class list {
public:
	list();
	~list();
public:
	// 初始化链表
	void initList();
	// 添加节点
	void addNode(NODE* newNode);
	// 返回节点
	NODE* getNode(uint8_t id);
	// 删除节点
	void removeNode(uint8_t id);
	// 清空链表
	void emptyList();
private:
	NODE* myHead;
};



#endif

