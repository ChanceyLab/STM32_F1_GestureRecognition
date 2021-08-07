#include <stm32f4xx.h>
#include "delay.h"
#include "oled.h"
#include "client_task.h"
void Nvic_Init(void);
void USART1_Configuration(void);
void FDC_Set(ClientID ID, uint16_t data);
	
uint32_t ch_data[4]={0};//ͨ������
uint8_t rx_buff[32] ={0};//���ջ���
uint8_t tx_buff[5] ={0};//���ͻ���
int main(void)
{
	delay_ms(1000);
	USART1_Configuration();//���ô���1
	Nvic_Init();
	
	OLED_Init();
	OLED_ShowString(0,0,"CH0:",16);
	OLED_ShowString(0,2,"CH1:",16);
	OLED_ShowString(0,4,"CH2:",16);
	OLED_ShowString(0,6,"CH3:",16);

	delay_ms(1000);
	//����ʱ�����������ü��ɡ�������ʹ��"client_task.h"ͷ�ļ�������ϸ˵��
	FDC_Set(reset_device, BOXEnable);//��λоƬ 
	FDC_Set(clk_src, Clk_src_internal);//ѡ���ڲ�ʱ��
	FDC_Set(sleep, BOXDisable);//�˳�˯��ģʽ ��λ��һ��Ҫ�˳�˯��ģʽ
	while(1)
	{//��ʾ����
		OLED_ShowNum(30,0,ch_data[0],9,16);//OLED��ʾ�ĸ�ͨ��������
		OLED_ShowNum(30,2,ch_data[1],9,16);
		OLED_ShowNum(30,4,ch_data[2],9,16);
		OLED_ShowNum(30,6,ch_data[3],9,16);
		delay_ms(50);
	}
}
/*
������һ��Ҫ����Ϊ115200��
���Ҫ��ֲ��51�ϣ�
1.��USART_GetITStatus(USART1, USART_IT_RXNE) != RESET������ΪRI,����if����β����RI =0��
2.��USART_ReceiveData(USART1)����ΪSBUF
*/
void USART1_IRQHandler(void)
{//����1�жϺ���
	uint8_t Res =0;
	static uint8_t StartRx =0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�ж��Ƿ�Ϊ�����ж�
	{//��
		Res = USART_ReceiveData(USART1);//��ȡ���ν��յ���һ���ֽ�����
		
		if(StartRx)//���������Ѿ���ʼ����֡����
		{
			rx_buff[StartRx -1] = Res;//����֡����
			
			if((StartRx -1) >= 17)
			{//�������
				StartRx = 0;//��Ǹ�λ
				if(rx_buff[16] != 0x55)//�ж�֡β�Ƿ�Ϊ 0x55������洢ͨ�����ݣ������������ν��գ�ֱ���˳�
					return;
				else
				{//�ϳ�ͨ������
					ch_data[0] = (rx_buff[0] * 16777216) + (rx_buff[1] * 65536) + (rx_buff[2] * 256) + rx_buff[3];
					ch_data[1] = (rx_buff[4] * 16777216) + (rx_buff[5] * 65536) + (rx_buff[6] * 256) + rx_buff[7];
					ch_data[2] = (rx_buff[8] * 16777216) + (rx_buff[9] * 65536) + (rx_buff[10] * 256) + rx_buff[11];
					ch_data[3] = (rx_buff[12] * 16777216) + (rx_buff[13] * 65536) + (rx_buff[14] * 256) + rx_buff[15];
				}
			}
			StartRx ++;//��ǵ��� Ϊ������һ�ֽ�������׼��
		}
		if((Res == 0x76) && (!StartRx))	//�����֡ͷ��δ��ʼ����ͨ������ 
			StartRx = 1;//��� ��һ�ν����жϿ�ʼ����֡����
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

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���ڿ����ж�

    USART_Cmd(USART1,ENABLE);

}

void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;//	/* NVIC_PriorityGroup */
   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//���ȼ�����2����ռ���ȼ���0-3��:��Ӧ���ȼ���0-3��
   
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//DMA2STR1CH5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

