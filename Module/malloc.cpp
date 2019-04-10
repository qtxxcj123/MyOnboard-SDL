#include "malloc.h"
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
__align(32) uint8_t mem1base[MEM1_MAX_SIZE];	//内部SRAM内存池
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
	if (ptr == NULL)return;//地址为0.  
	offset = (uint32_t)ptr - (uint32_t)mallco_dev.membase;
	my_mem_free(offset);	//释放内存   
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
		mymemcpy((void*)((uint32_t)mallco_dev.membase + offset), ptr, size);	//拷贝旧内存内容到新内存   
		myfree(ptr);  											  		//释放旧内存
		return (void*)((uint32_t)mallco_dev.membase + offset);  				//返回新内存首地址
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
	mymemset(mallco_dev.memmap, 0, memtblsize * 2);//内存状态表数据清零  
	mymemset(mallco_dev.membase, 0, memsize);	//内存池所有数据清零  
	mallco_dev.memrdy = true;								//内存管理初始化OK  
}

uint32_t MyMMU::my_mem_malloc(uint32_t size)
{
	signed long offset = 0;
	uint32_t nmemb;	//需要的内存块数  
	uint32_t cmemb = 0;//连续空内存块数
	uint32_t i;
	if (!mallco_dev.memrdy)my_mem_init();//未初始化,先执行初始化 
	if (size == 0)return 0XFFFFFFFF;//不需要分配
	nmemb = size / memblksize;  	//获取需要分配的连续内存块数
	if (size%memblksize)nmemb++;
	for (offset = memtblsize - 1; offset >= 0; offset--)//搜索整个内存控制区  
	{
		if (!mallco_dev.memmap[offset])cmemb++;//连续空内存块数增加
		else cmemb = 0;								//连续内存块清零
		if (cmemb == nmemb)							//找到了连续nmemb个空内存块
		{
			for (i = 0; i < nmemb; i++)  					//标注内存块非空 
			{
				mallco_dev.memmap[offset + i] = nmemb;
			}
			return (offset*memblksize);//返回偏移地址  
		}
	}
	return 0XFFFFFFFF;//未找到符合分配条件的内存块
}

uint8_t MyMMU::my_mem_free(uint32_t offset)
{
	int i;
	if (!mallco_dev.memrdy)//未初始化,先执行初始化
	{
		my_mem_init();
		return 1;//未初始化  
	}
	if (offset < memsize)//偏移在内存池内. 
	{
		int index = offset / memblksize;			//偏移所在内存块号码  
		int nmemb = mallco_dev.memmap[index];	//内存块数量
		for (i = 0; i < nmemb; i++)  						//内存块清零
		{
			mallco_dev.memmap[index + i] = 0;
		}
		return 0;
	}
	else return 2;//偏移超区了.  
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
