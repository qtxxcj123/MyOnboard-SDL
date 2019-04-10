#include "listLinked.h"
listLinked::listLinked()
{
	this->head = new NODE();
	initList();
}

listLinked::~listLinked()
{
}

void listLinked::initList()
{
	if (this->head == NULL)return;
	this->head->id = 0;
	this->head->next = this->head;
	this->head->prior = this->head;
}

void listLinked::addNode(NODE * newNode)
{
	if (!this->head || !newNode)return;
	list_add_tail(newNode, this->head);
}

void listLinked::removeNode(uint8_t id)
{
	if (this->head == NULL)return;
	if (this->head->next == this->head)return;
	NODE* firstNode = this->head->next;
	NODE* endNode = this->head->prior;
	while (firstNode != this->head && endNode != this->head)
	{
		if (firstNode->id == id)
		{
			firstNode->next->prior = firstNode->prior;
			firstNode->prior->next = firstNode->next;
			//TODO：需要提供单片机内存删除方法
			myfree(SRAMIN,firstNode);
			return;
		}
		else if (endNode->id == id)
		{
			endNode->next->prior = endNode->prior;
			endNode->prior->next = endNode->next;
			//TODO：需要提供单片机内存删除方法
			myfree(SRAMIN,firstNode);
			return;
		}
		firstNode = firstNode->next;
		endNode = endNode->prior;
	}
}

void listLinked::replaceNode(uint8_t id, NODE * newNode)
{
	if (this->head == NULL)return;
	if (this->head->next == this->head)return;
	NODE* firstNode = this->head->next;
	NODE* endNode = this->head->prior;
	while (firstNode != this->head && endNode != this->head)
	{
		if (firstNode->id == id)
		{
			list_replace(firstNode,newNode);
			myfree(SRAMIN,firstNode);
			break;
		}
		if (endNode->id == id)
		{
			list_replace(endNode, newNode);
			myfree(SRAMIN,firstNode);
			break;
		}
		firstNode = firstNode->next;
		endNode = endNode->prior;
	}
}

NODE * listLinked::getNode(uint8_t id)
{
	if (this->head == NULL)return nullptr;
	if (this->head->next == this->head)return nullptr;
	NODE* firstNode = this->head->next;
	NODE* endNode = this->head->prior;
	while (firstNode != this->head && endNode != this->head)
	{
		if (firstNode->id == id)
		{
			return firstNode;
		}
		if (endNode->id == id)
		{
			return endNode;
		}
		firstNode = firstNode->next;
		endNode = endNode->prior;
	}
	return nullptr;
}

inline void listLinked::list_add(NODE * newNode, NODE * head)
{
	__list_add(newNode, this->head, this->head->next);
}

inline void listLinked::list_add_tail(NODE * newNode, NODE * head)
{
	__list_add(newNode, this->head->prior, this->head);
}

inline void listLinked::__list_add(NODE * newNode, NODE * prev, NODE * next)
{
	next->prior = newNode;
	newNode->next = next;
	newNode->prior = prev;
	prev->next = newNode;
}
inline void listLinked::list_replace(NODE* oldNode,
	NODE* newNode)
{
	newNode->next = oldNode->next;
	newNode->next->prior = newNode;
	newNode->prior = oldNode->prior;
	newNode->prior->next = newNode;
}


#ifdef LINUX

inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *new,
	struct list_head *prev,
	struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *this->head)
{
	__list_add(new, this->head, this->head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *this->head)
{
	__list_add(new, this->head->prev, this->head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void list_del_init(struct list_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}

static inline void list_replace(struct list_head *old,
	struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline int list_empty(const struct list_head *this->head)
{
	return this->head->next == this->head;
}
#endif