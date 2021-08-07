#include <stm32f4xx.h>
#include "delay.h"
#include "oled.h"
#include "client_task.h"
void Nvic_Init(void);
void USART1_Configuration(void);
void FDC_Set(ClientID ID, uint16_t data);
	
uint32_t ch_data[4]={0};//通道数据
uint8_t rx_buff[32] ={0};//接收缓冲
uint8_t tx_buff[5] ={0};//发送缓冲
int main(void)
{
	delay_ms(1000);
	USART1_Configuration();//配置串口1
	Nvic_Init();
	
	OLED_Init();
	OLED_ShowString(0,0,"CH0:",16);
	OLED_ShowString(0,2,"CH1:",16);
	OLED_ShowString(0,4,"CH2:",16);
	OLED_ShowString(0,6,"CH3:",16);

	delay_ms(1000);
	//配置时按照下面配置即可、、具体使用"client_task.h"头文件内有详细说明
	FDC_Set(reset_device, BOXEnable);//复位芯片 
	FDC_Set(clk_src, Clk_src_internal);//选用内部时钟
	FDC_Set(sleep, BOXDisable);//退出睡眠模式 复位后一定要退出睡眠模式
	while(1)
	{//显示数据
		OLED_ShowNum(30,0,ch_data[0],9,16);//OLED显示四个通道的数据
		OLED_ShowNum(30,2,ch_data[1],9,16);
		OLED_ShowNum(30,4,ch_data[2],9,16);
		OLED_ShowNum(30,6,ch_data[3],9,16);
		delay_ms(50);
	}
}
/*
波特率一定要设置为115200·
如果要移植到51上：
1.把USART_GetITStatus(USART1, USART_IT_RXNE) != RESET这句更改为RI,并在if语句结尾加上RI =0；
2.把USART_ReceiveData(USART1)更换为SBUF
*/
void USART1_IRQHandler(void)
{//串口1中断函数
	uint8_t Res =0;
	static uint8_t StartRx =0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//判断是否为接收中断
	{//是
		Res = USART_ReceiveData(USART1);//读取本次接收到的一个字节数据
		
		if(StartRx)//如果标记了已经开始接收帧数据
		{
			rx_buff[StartRx -1] = Res;//接收帧数据
			
			if((StartRx -1) >= 17)
			{//接收完毕
				StartRx = 0;//标记复位
				if(rx_buff[16] != 0x55)//判断帧尾是否为 0x55，是则存储通道数据；否则舍弃本次接收，直接退出
					return;
				else
				{//合成通道数据
					ch_data[0] = (rx_buff[0] * 16777216) + (rx_buff[1] * 65536) + (rx_buff[2] * 256) + rx_buff[3];
					ch_data[1] = (rx_buff[4] * 16777216) + (rx_buff[5] * 65536) + (rx_buff[6] * 256) + rx_buff[7];
					ch_data[2] = (rx_buff[8] * 16777216) + (rx_buff[9] * 65536) + (rx_buff[10] * 256) + rx_buff[11];
					ch_data[3] = (rx_buff[12] * 16777216) + (rx_buff[13] * 65536) + (rx_buff[14] * 256) + rx_buff[15];
				}
			}
			StartRx ++;//标记递增 为接收下一字节数据做准备
		}
		if((Res == 0x76) && (!StartRx))	//如果是帧头且未开始接收通道数据 
			StartRx = 1;//标记 下一次进入中断开始接收帧数据
  }
}
void FDC_Set(ClientID ID, uint16_t data)
{
	uint8_t i;
	tx_buff[0] = 0x56;
	tx_buff[1] = (uint8_t)(ID);
	tx_buff[2] = (uint8_t)(data >> 8);
	tx_buff[3] = (uint8_t)(data);
	tx_buff[4] = 0x55;
	for(i=0;i< 5; i++)
	{
		USART_SendData(USART1,tx_buff[i]);
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	}
}

void USART1_Configuration(void)
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6 ,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7 ,GPIO_AF_USART1);

    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&gpio);

    usart.USART_BaudRate = 115200;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1; 
    usart.USART_Parity = USART_Parity_No;
    usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&usart);

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//串口空闲中断

    USART_Cmd(USART1,ENABLE);

}

void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;//	/* NVIC_PriorityGroup */
   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//优先级分组2：抢占优先级（0-3）:相应优先级（0-3）
   
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//DMA2STR1CH5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

