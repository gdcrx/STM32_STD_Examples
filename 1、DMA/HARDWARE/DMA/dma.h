#ifndef __DMA_H

#define __DMA_H
#include "stm32f4xx.h" 

 
//配置DMAx_CHx
void STD_DMA_Init(DMA_Stream_TypeDef *DMA_Streamx,u32 Channel,u32 PeriphAddress,u32 MemoryAddress,u16 DataLength);

//开启一次DMA传输	
void STD_DMA_Start(DMA_Stream_TypeDef *DMA_Streamx,u16 DataLength);	

#endif
