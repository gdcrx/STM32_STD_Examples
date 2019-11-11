
#include "dma.h"

/*MYDMA_Config配置初始化DMA函数
DMAx:					DMA1 ->0x40026000	, DMA2 -> 0x40026400
DMA_Streamx:	DMA数据流
Channel：			DMA通道
PeriphAddress:					DMA外设地址
MemoryAddress:					存储器标地址
DataLength:				传输数据长度
*/
void STD_DMA_Init(DMA_Stream_TypeDef *DMA_Streamx,u32 Channel,u32 PeriphAddress,u32 MemoryAddress,u16 DataLength)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	//RCC_AHB1PeriphClockCmd((u32)DMAx,ENABLE);	//DMA时钟使能  
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
	// 恢复DMA初始值
	DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	
  DMA_InitStructure.DMA_Channel = Channel;  //通
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_PeripheralBaseAddr = PeriphAddress;//DMA 外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = MemoryAddress;//DMA 存储器地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = DataLength;								//数据传输长度 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream 

}

 
/**
  * @brief  Starts the DMA Transfer. 开启DMA传输
  * @param  hdma      : pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream. 
  * @param  DataLength: The length of data to be transferred from source to destination
  * @retval void
  */
void STD_DMA_Start(DMA_Stream_TypeDef *DMA_Streamx,u16 DataLength)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,DataLength);     //数据传输长度
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	  
