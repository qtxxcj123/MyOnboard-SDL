#ifndef DMA_H
#define DMA_H
#include "stm32f4xx.h" 

#define BUFF_SIZE 40
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
void DMA_Config();
void DMA_Enable(uint8_t len);
void DMA_Start(uint8_t len);
#ifdef __cplusplus
}
#endif //__cplusplus


#endif