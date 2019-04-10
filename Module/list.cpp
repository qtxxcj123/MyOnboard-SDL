#include "dji_vehicle.hpp"
list::list()
{
	initList();
}

list::~list()
{
}

void list::initList()
{
	myHead = new NODE();
	if (!myHead)return;
	myHead->id = 0;
	myHead->next = NULL;
}

void list::addNode(NODE * newNode)
{
	NODE* cur = myHead->next; // 当前节点current
	NODE* pre = myHead;  // 上一个节点previous
	while (cur)
	{
		if (newNode->id < cur->id) // 找到这个位置
			break;
		pre = cur;
		cur = cur->next;  // 找到最后一个对象
	}
	// 插入到pre节点的后面
	newNode->next = pre->next;
	pre->next = newNode;
}

NODE * list::getNode(uint8_t id)
{
	NODE* pre = myHead->next;
	while (pre)
	{
		if (pre->id == id)
			return pre;
		pre = pre->next;
	}
	return NULL;
}

void list::removeNode(uint8_t id)
{
	NODE* cur = myHead->next; // 当前节点current
	NODE* pre = myHead;  // 上一个节点previous
	while (cur)
	{
		if (id == cur->id) // 找到这个位置
		{
			// 删除该节点
			pre->next = cur->next;
			delete cur->data;
			delete cur;
//			myfree(SRAMIN,cur->data);
//			myfree(SRAMIN,cur);
//			v->mmu->myfree(pre->data);
//			v->mmu->myfree(pre);
			break;
		}
		pre = cur;
		cur = cur->next;  // 找到最后一个对象
	}

}

void list::emptyList()
{
	NODE* pre = myHead->next;
	NODE* p;
	while (pre)
	{
		p = pre->next;
		//TODO : 删除的方法
		delete pre->data;
		delete pre;
//		myfree(SRAMIN,pre->data);
//		myfree(SRAMIN,pre);
//		v->mmu->myfree(pre->data);
//		v->mmu->myfree(pre);
		pre = p;
	}
	myHead->next = NULL;
}
