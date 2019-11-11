#ifndef __DMA_H

#define __DMA_H
#include "stm32f4xx.h" 

 
//����DMAx_CHx
void STD_DMA_Init(DMA_Stream_TypeDef *DMA_Streamx,u32 Channel,u32 PeriphAddress,u32 MemoryAddress,u16 DataLength);

//����һ��DMA����	
void STD_DMA_Start(DMA_Stream_TypeDef *DMA_Streamx,u16 DataLength);	

#endif
