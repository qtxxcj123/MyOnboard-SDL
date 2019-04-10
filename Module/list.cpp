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
	NODE* cur = myHead->next; // ��ǰ�ڵ�current
	NODE* pre = myHead;  // ��һ���ڵ�previous
	while (cur)
	{
		if (newNode->id < cur->id) // �ҵ����λ��
			break;
		pre = cur;
		cur = cur->next;  // �ҵ����һ������
	}
	// ���뵽pre�ڵ�ĺ���
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
	NODE* cur = myHead->next; // ��ǰ�ڵ�current
	NODE* pre = myHead;  // ��һ���ڵ�previous
	while (cur)
	{
		if (id == cur->id) // �ҵ����λ��
		{
			// ɾ���ýڵ�
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
		cur = cur->next;  // �ҵ����һ������
	}

}

void list::emptyList()
{
	NODE* pre = myHead->next;
	NODE* p;
	while (pre)
	{
		p = pre->next;
		//TODO : ɾ���ķ���
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
