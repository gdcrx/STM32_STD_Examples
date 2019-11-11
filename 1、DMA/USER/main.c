#include "sys.h"
#include "usart.h"
#include "dma.h" 

 
#define SEND_BUF_SIZE 8204	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.

u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
const u8 TEXT_TO_SEND[]={"STM32F407IGTx DMA ����ʵ��"};	 

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
	// LED�ƽӵ�PE5,PE6����
  GPIO_Init(GPIOE, &GPIO_InitStructure);

	// LED�ƽӵ�PH9����
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
	USART1_Init(115200);
		
	txtLen = sizeof(TEXT_TO_SEND);  
	
	// ��STM32F407IGTx�У�USART1�����ݷ���ʹ��������7��DMAͨ��4
	STD_DMA_Init(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);
	
	for(i=0;i<SEND_BUF_SIZE;i++) 
	{
		if(t>=txtLen)//���뻻�з�
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
		}else//����TEXT_TO_SEND���
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];
			t++;
		}   	   
	}		
 
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����     
	STD_DMA_Start(DMA2_Stream7,SEND_BUF_SIZE);     //��ʼһ��DMA���䣡	  
	//�ȴ�DMA�������	 
	while(1)
	{
		if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
		{ 
			DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־
			break; 
		}
		
		pro=DMA_GetCurrDataCounter(DMA2_Stream7);//�õ���ǰ��ʣ����ٸ�����
		pro=1-pro/SEND_BUF_SIZE;//�õ��ٷֱ�	  
		pro*=100;      			    //����100��
	}
	
	while(1)
	{
	}
}
