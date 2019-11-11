#include "sys.h"
#include "usart.h"
#include "dma.h" 

 
#define SEND_BUF_SIZE 8204	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.

u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
const u8 TEXT_TO_SEND[]={"STM32F407IGTx DMA 串口实验"};	 

void ST_GPIO_Init(void);
	
void ST_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// LED灯接的PE5,PE6引脚
  GPIO_Init(GPIOE, &GPIO_InitStructure);

	// LED灯接的PH9引脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOH, &GPIO_InitStructure);
	
}

int main(void)
{
	u16 i;
	u8 t =0, mask = 0;
	u8 txtLen ;
	float pro = 0.0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	
	USART1_Init(115200);
		
	txtLen = sizeof(TEXT_TO_SEND);  
	
	// 在STM32F407IGTx中，USART1的数据发送使用数据流7，DMA通道4
	STD_DMA_Init(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);
	
	for(i=0;i<SEND_BUF_SIZE;i++) 
	{
		if(t>=txtLen)//加入换行符
		{
			if(mask)
			{
				SendBuff[i]=0x0a;
				t=0;
			}else 
			{
				SendBuff[i]=0x0d;
				mask++;
			}	
		}else//复制TEXT_TO_SEND语句
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];
			t++;
		}   	   
	}		
 
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送     
	STD_DMA_Start(DMA2_Stream7,SEND_BUF_SIZE);     //开始一次DMA传输！	  
	//等待DMA传输完成	 
	while(1)
	{
		if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
		{ 
			DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志
			break; 
		}
		
		pro=DMA_GetCurrDataCounter(DMA2_Stream7);//得到当前还剩余多少个数据
		pro=1-pro/SEND_BUF_SIZE;//得到百分比	  
		pro*=100;      			    //扩大100倍
	}
	
	while(1)
	{
	}
}
