
#include "dma.h"

/*MYDMA_Config���ó�ʼ��DMA����
DMAx:					DMA1 ->0x40026000	, DMA2 -> 0x40026400
DMA_Streamx:	DMA������
Channel��			DMAͨ��
PeriphAddress:					DMA�����ַ
MemoryAddress:					�洢�����ַ
DataLength:				�������ݳ���
*/
void STD_DMA_Init(DMA_Stream_TypeDef *DMA_Streamx,u32 Channel,u32 PeriphAddress,u32 MemoryAddress,u16 DataLength)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	//RCC_AHB1PeriphClockCmd((u32)DMAx,ENABLE);	//DMAʱ��ʹ��  
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
	// �ָ�DMA��ʼֵ
	DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
	
  DMA_InitStructure.DMA_Channel = Channel;  //ͨ
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_PeripheralBaseAddr = PeriphAddress;//DMA �����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = MemoryAddress;//DMA �洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = DataLength;								//���ݴ��䳤�� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream 

}

 
/**
  * @brief  Starts the DMA Transfer. ����DMA����
  * @param  hdma      : pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream. 
  * @param  DataLength: The length of data to be transferred from source to destination
  * @retval void
  */
void STD_DMA_Start(DMA_Stream_TypeDef *DMA_Streamx,u16 DataLength)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA_Streamx,DataLength);     //���ݴ��䳤��
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
}	  
