#ifndef _MALLOC_H
#define _MALLOC_H
#include <stdint.h>
#ifndef NULL
#define NULL 0
#endif
#define MEM1_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE			50*1024  						//�������ڴ� 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С
class MyMMU {
private:
	struct _m_mallco_dev{
		uint8_t 	*membase;				//�ڴ�� ����SRAMBANK��������ڴ�
		uint16_t *memmap; 				//�ڴ����״̬��
		bool  memrdy; 				//�ڴ�����Ƿ����
	};
public:
	MyMMU();
	~MyMMU();
public:
	void myfree(void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
	void *mymalloc(uint32_t size);			//�ڴ����(�ⲿ����)
	void *myrealloc(void *ptr, uint8_t size);//���·����ڴ�(�ⲿ����)
	uint8_t my_mem_perused();				//����ڴ�ʹ����(��/�ڲ�����) 
private:
	void mymemset(void *s, uint8_t c, uint32_t count);	//�����ڴ�
	void mymemcpy(void *des, void *src, uint32_t n);//�����ڴ�     
	void my_mem_init();				//�ڴ�����ʼ������(��/�ڲ�����)
	uint32_t my_mem_malloc(uint32_t size);	//�ڴ����(�ڲ�����)
	uint8_t my_mem_free(uint32_t offset);		//�ڴ��ͷ�(�ڲ�����)
private:
	uint16_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];
	const uint32_t memtblsize;	//�ڴ���С
	const uint32_t memblksize;					//�ڴ�ֿ��С
	const uint32_t memsize;							//�ڴ��ܴ�С
	_m_mallco_dev mallco_dev;
};

#endif
