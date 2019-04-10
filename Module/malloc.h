#ifndef _MALLOC_H
#define _MALLOC_H
#include <stdint.h>
#ifndef NULL
#define NULL 0
#endif
#define MEM1_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM1_MAX_SIZE			50*1024  						//最大管理内存 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小
class MyMMU {
private:
	struct _m_mallco_dev{
		uint8_t 	*membase;				//内存池 管理SRAMBANK个区域的内存
		uint16_t *memmap; 				//内存管理状态表
		bool  memrdy; 				//内存管理是否就绪
	};
public:
	MyMMU();
	~MyMMU();
public:
	void myfree(void *ptr);  			//内存释放(外部调用)
	void *mymalloc(uint32_t size);			//内存分配(外部调用)
	void *myrealloc(void *ptr, uint8_t size);//重新分配内存(外部调用)
	uint8_t my_mem_perused();				//获得内存使用率(外/内部调用) 
private:
	void mymemset(void *s, uint8_t c, uint32_t count);	//设置内存
	void mymemcpy(void *des, void *src, uint32_t n);//复制内存     
	void my_mem_init();				//内存管理初始化函数(外/内部调用)
	uint32_t my_mem_malloc(uint32_t size);	//内存分配(内部调用)
	uint8_t my_mem_free(uint32_t offset);		//内存释放(内部调用)
private:
	uint16_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];
	const uint32_t memtblsize;	//内存表大小
	const uint32_t memblksize;					//内存分块大小
	const uint32_t memsize;							//内存总大小
	_m_mallco_dev mallco_dev;
};

#endif
