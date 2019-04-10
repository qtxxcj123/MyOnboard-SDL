#ifndef _LIST_H
#define _LIST_H
#include "myType.h"
#include <stdint.h>
#include <stdio.h>


class listLinked {
public:
	listLinked();
	~listLinked();
public:
	void initList();

	void addNode(NODE* newNode);

	void removeNode(uint8_t id);

	void replaceNode(uint8_t id, NODE* newNode);

	NODE* getNode(uint8_t id);

	inline void list_add(NODE *newNode, NODE *head);

	inline void list_add_tail(NODE *newNode, NODE *head);
private:
	static inline void __list_add(NODE *newNode,
		NODE *prev,
		NODE *next);
		static inline void list_replace(NODE * oldNode, NODE * newNode);
private:
	NODE* head;
};


#ifdef LINUX
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)   //��ýṹ��(TYPE)�ı�����Ա(MEMBER)�ڴ˽ṹ���е�ƫ������
/*< ����"�ṹ��(type)����"�е�"���Ա����(member)��ָ��(ptr)"����ȡָ�������ṹ�������ָ�롣>*/
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})		

#define LIST_HEAD_INIT(name) { &(name), &(name) }  // init list

#define LIST_HEAD(name) \
    NODE name = LIST_HEAD_INIT(name)

#define list_entry(ptr, type, member) \			// ��ȡ�ڵ�
container_of(ptr, type, member)

#define list_for_each(pos, head) \				// �����ڵ�
for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)
#endif 

#endif
