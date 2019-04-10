#include "malloc.h"
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
__align(32) uint8_t mem1base[MEM1_MAX_SIZE];	//�ڲ�SRAM�ڴ��
#ifdef __cplusplus
}
#endif //__cplusplus

MyMMU::MyMMU():
	memtblsize(MEM1_ALLOC_TABLE_SIZE)
	, memblksize(MEM1_BLOCK_SIZE)
	, memsize(MEM1_MAX_SIZE)
{
	mallco_dev.membase = mem1base;
	mallco_dev.memmap = mem1mapbase;
	mallco_dev.memrdy = false;
	my_mem_init();
}

MyMMU::~MyMMU()
{
}

void MyMMU::myfree(void * ptr)
{
	uint32_t offset;
	if (ptr == NULL)return;//��ַΪ0.  
	offset = (uint32_t)ptr - (uint32_t)mallco_dev.membase;
	my_mem_free(offset);	//�ͷ��ڴ�   
}

void * MyMMU::mymalloc(uint32_t size)
{
	uint32_t offset;
	offset = my_mem_malloc(size);
	if (offset == 0XFFFFFFFF)return NULL;
	else return (void*)((uint32_t)mallco_dev.membase + offset);
}

void * MyMMU::myrealloc(void * ptr, uint8_t size)
{
	uint32_t offset;
	offset = my_mem_malloc(size);
	if (offset == 0XFFFFFFFF)return NULL;
	else
	{
		mymemcpy((void*)((uint32_t)mallco_dev.membase + offset), ptr, size);	//�������ڴ����ݵ����ڴ�   
		myfree(ptr);  											  		//�ͷž��ڴ�
		return (void*)((uint32_t)mallco_dev.membase + offset);  				//�������ڴ��׵�ַ
	}
}

void MyMMU::mymemset(void * s, uint8_t c, uint32_t count)
{
	uint8_t *xs = (uint8_t *)s;
	while (count--)*xs++ = c;
}

void MyMMU::mymemcpy(void * des, void * src, uint32_t n)
{
	uint8_t *xdes = (uint8_t *)des;
	uint8_t *xsrc = (uint8_t *)src;
	while (n--)*xdes++ = *xsrc++;
}

void MyMMU::my_mem_init()
{
	mymemset(mallco_dev.memmap, 0, memtblsize * 2);//�ڴ�״̬����������  
	mymemset(mallco_dev.membase, 0, memsize);	//�ڴ��������������  
	mallco_dev.memrdy = true;								//�ڴ�����ʼ��OK  
}

uint32_t MyMMU::my_mem_malloc(uint32_t size)
{
	signed long offset = 0;
	uint32_t nmemb;	//��Ҫ���ڴ����  
	uint32_t cmemb = 0;//�������ڴ����
	uint32_t i;
	if (!mallco_dev.memrdy)my_mem_init();//δ��ʼ��,��ִ�г�ʼ�� 
	if (size == 0)return 0XFFFFFFFF;//����Ҫ����
	nmemb = size / memblksize;  	//��ȡ��Ҫ����������ڴ����
	if (size%memblksize)nmemb++;
	for (offset = memtblsize - 1; offset >= 0; offset--)//���������ڴ������  
	{
		if (!mallco_dev.memmap[offset])cmemb++;//�������ڴ��������
		else cmemb = 0;								//�����ڴ������
		if (cmemb == nmemb)							//�ҵ�������nmemb�����ڴ��
		{
			for (i = 0; i < nmemb; i++)  					//��ע�ڴ��ǿ� 
			{
				mallco_dev.memmap[offset + i] = nmemb;
			}
			return (offset*memblksize);//����ƫ�Ƶ�ַ  
		}
	}
	return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��
}

uint8_t MyMMU::my_mem_free(uint32_t offset)
{
	int i;
	if (!mallco_dev.memrdy)//δ��ʼ��,��ִ�г�ʼ��
	{
		my_mem_init();
		return 1;//δ��ʼ��  
	}
	if (offset < memsize)//ƫ�����ڴ����. 
	{
		int index = offset / memblksize;			//ƫ�������ڴ�����  
		int nmemb = mallco_dev.memmap[index];	//�ڴ������
		for (i = 0; i < nmemb; i++)  						//�ڴ������
		{
			mallco_dev.memmap[index + i] = 0;
		}
		return 0;
	}
	else return 2;//ƫ�Ƴ�����.  
}

uint8_t MyMMU::my_mem_perused()
{
	uint32_t used = 0;
	uint32_t i;
	for (i = 0; i < memtblsize; i++)
	{
		if (mallco_dev.memmap[i])used++;
	}
	return (used * 100) / (memtblsize);
}
