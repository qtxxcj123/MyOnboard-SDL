#ifndef _EFUNS_H
#define _EFUNS_H
#include "ff.h"
#include "stm32f4xx.h"
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

FRESULT initFatfs(const char *name);

FRESULT readFile(void * buffer, uint32_t len);

FRESULT writeFile(void * buffer, uint32_t len);

FRESULT closeFile();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
