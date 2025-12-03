#include "USART.h"

char message[200];
uint8_t flag=0;
void USART_DMAConfig(void ){
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1 ,ENABLE );
	
	DMA_InitTypeDef DMAStruct;
	DMAStruct.DMA_PeripheralBaseAddr =(uint32_t)&USART1->DR ;
	DMAStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte ;
	DMAStruct.DMA_PeripheralInc =DMA_PeripheralInc_Disable ;
	
	DMAStruct.DMA_MemoryBaseAddr =(uint32_t ) message ;
	DMAStruct.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte ;
	DMAStruct.DMA_MemoryInc =DMA_MemoryInc_Enable ;
		
	DMAStruct.DMA_BufferSize =BUFFSIZE ;
	DMAStruct.DMA_DIR =DMA_DIR_PeripheralSRC ;
	DMAStruct.DMA_M2M =DMA_M2M_Disable ;
	DMAStruct.DMA_Mode =DMA_Mode_Normal ;
	DMAStruct.DMA_Priority =DMA_Priority_High ;

	DMA_Init (DMA1_Channel5 ,&DMAStruct );
	
	DMA_Cmd (DMA1_Channel5 ,ENABLE );
}

void USARTInit(void){

	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 ,ENABLE );
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE );
	
	GPIO_InitTypeDef GPIOStruct;
	GPIOStruct.GPIO_Mode =GPIO_Mode_AF_PP ;
	GPIOStruct.GPIO_Pin =GPIO_Pin_9 ;
	GPIOStruct.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA ,& GPIOStruct);
	
	GPIOStruct.GPIO_Mode =GPIO_Mode_IN_FLOATING  ;
	GPIOStruct.GPIO_Pin =GPIO_Pin_10 ;
	GPIO_Init (GPIOA ,& GPIOStruct);
	
	
	USART_InitTypeDef USARTStruct;
	USARTStruct.USART_BaudRate =9600;
	USARTStruct.USART_HardwareFlowControl =USART_HardwareFlowControl_None ;
	USARTStruct.USART_Mode =USART_Mode_Rx |USART_Mode_Tx ;
	USARTStruct.USART_Parity =USART_Parity_No ;
	USARTStruct.USART_StopBits =USART_StopBits_1 ;
	USARTStruct.USART_WordLength =USART_WordLength_8b ;
	USART_Init (USART1 ,&USARTStruct );
	

		//DMA配置
	USART_DMAConfig();
	//使能usart_dma
	USART_DMACmd (USART1 ,USART_DMAReq_Rx ,ENABLE );
	
	USART_ITConfig (USART1 ,USART_IT_RXNE ,ENABLE );
	//空闲中断
	USART_ITConfig (USART1,USART_IT_IDLE,ENABLE  );
	
	NVIC_InitTypeDef NVICStruct;
	NVICStruct.NVIC_IRQChannel =USART1_IRQn ;
	NVICStruct.NVIC_IRQChannelCmd =ENABLE ;
	NVICStruct.NVIC_IRQChannelPreemptionPriority =1;
	NVICStruct.NVIC_IRQChannelSubPriority =1;
	
	NVIC_Init (&NVICStruct );
	

	
USART_Cmd (USART1 ,ENABLE );
	
	
}
void Send_Byte(uint8_t data){
		while(USART_GetFlagStatus (USART1 ,USART_FLAG_TXE )==0);
		USART_SendData (USART1,data);

}
void Send_Arr(uint8_t *arr,uint16_t len){
		for(uint16_t i=0;i<len;i++){
				Send_Byte (arr[i]);		
		}

}
void Send_String(char  *arr){
		while(*arr){
				Send_Byte (*arr++);	
		}

}
int fputc(int ch,FILE *f){
		Send_Byte (ch);
		return ch;
	
}

//都到这个中端
void USART1_IRQHandler(void){
//		static uint16_t index=0;
//		if(USART_GetITStatus (USART1 ,USART_IT_RXNE )==SET){
//				
//				uint8_t data=USART_ReceiveData (USART1 );
//				message [index++]=data;
//				
//			
//					USART_ClearITPendingBit (USART1,USART_IT_RXNE );
//		}
		 if(USART_GetITStatus (USART1 ,USART_IT_IDLE )==SET ){
				
					message [BUFFSIZE -DMA_GetCurrDataCounter (DMA1_Channel5)]='\0';
					flag=1;
					USART_ClearITPendingBit (USART1,USART_IT_IDLE );
					uint32_t temp;
					temp=USART1 ->SR ;
					temp=USART1 ->DR;
					(void)temp;
			 
					DMA_Cmd (DMA1_Channel5 ,DISABLE );
					DMA_SetCurrDataCounter (DMA1_Channel5 ,BUFFSIZE );
					DMA_Cmd(DMA1_Channel5 ,ENABLE );
		}

}


