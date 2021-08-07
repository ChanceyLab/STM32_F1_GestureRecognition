#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "oled.h"
#include "fdc2214.h"
#include "McuDataSend.h"

void Sys_Init(void);

float res0,res1,res2,res3;
float temp0,temp1,temp2,temp3;
int main(void)
{
  Sys_Init();
	while(1)
	{int i=0;
		i=i+1;
//		printf("%d \r\n",i);
		printf("CH0;%3.3f \r\n",res0-temp0);
		//printf("%3.3f \r\n",res0-temp0);//��ʼֵΪ��·���ݣ���ȥ��·����Ϊ��������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		delay_ms(5000);
	}
}

void Sys_Init(void)
{
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration();//����NVIC�жϷ���2:2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	LED_Init();
	while(FDC2214_Init());
	led=0;
	delay_ms(1000);
	temp0 = Cap_Calculate(0);//��ȡ��ʼֵ
	temp1 = Cap_Calculate(1);
	temp2 = Cap_Calculate(2);
	temp3 = Cap_Calculate(3);
	TIM5_Ctrl_Init(500-1,7200-1);   //50ms�ɼ�һ�Σ�500/10
}

//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)   //TIM5�ж�
{
	if(TIM5->SR&0X0001)//50ms��ʱ�ж�
 {
	res0 = Cap_Calculate(0);//�ɼ�����
	res1 = Cap_Calculate(1);
	res2 = Cap_Calculate(2);
	res3 = Cap_Calculate(3);
//	printf("%5d : %5d : %5d\r\n",res0,res1,res2);
	 led=~led;
 }
 TIM5->SR&=~(1<<0);  //�жϱ�־λ��0���ȴ���һ���ж�
}
