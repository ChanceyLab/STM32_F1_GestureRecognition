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
		//printf("%3.3f \r\n",res0-temp0);//初始值为线路电容，减去线路电容为检测物电容
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		delay_ms(5000);
	}
}

void Sys_Init(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration();//设置NVIC中断分组2:2；2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	LED_Init();
	while(FDC2214_Init());
	led=0;
	delay_ms(1000);
	temp0 = Cap_Calculate(0);//读取初始值
	temp1 = Cap_Calculate(1);
	temp2 = Cap_Calculate(2);
	temp3 = Cap_Calculate(3);
	TIM5_Ctrl_Init(500-1,7200-1);   //50ms采集一次，500/10
}

//定时器5中断服务程序
void TIM5_IRQHandler(void)   //TIM5中断
{
	if(TIM5->SR&0X0001)//50ms定时中断
 {
	res0 = Cap_Calculate(0);//采集数据
	res1 = Cap_Calculate(1);
	res2 = Cap_Calculate(2);
	res3 = Cap_Calculate(3);
//	printf("%5d : %5d : %5d\r\n",res0,res1,res2);
	 led=~led;
 }
 TIM5->SR&=~(1<<0);  //中断标志位清0，等待下一次中断
}
