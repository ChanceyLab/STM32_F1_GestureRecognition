#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "fdc2214.h"
#include "IIC.h"
//宏定义初始值
#define ch0_init 61294
#define ch1_init 61043
#define ch2_init 59246
#define ch3_init 62089

void Sys_Init(void);

float res0,res1,res2,res3;
float temp0,temp1,temp2,temp3;
int val_ch0,val_ch1,val_ch2,val_ch3;
u8 date_process()
{
	val_ch0 = (1000 * (res0-temp0));val_ch0 -= val_ch0%100;
	val_ch1 = (1000 * (res1-temp1));val_ch1 -= val_ch1%100;
	val_ch2 = (1000 * (res2-temp2));val_ch2 -= val_ch2%100;
	val_ch3 = (1000 * (res3-temp3));val_ch3 -= val_ch3%100;
}	
/*
需要写一个初始值判定函数看看初始是否有手在上面
*/
int main(void)
{
  Sys_Init();
	led_init();
	while(1)
	{
		int i=0;
		date_process();
				date_process();

				date_process();

				date_process();

		i=i+1;
//判定
		if(val_ch0>2100 && val_ch1>900 && val_ch2>=700 && val_ch3>1400)
		{
			printf("55555555555555555555555555555555555555！！！\r\n");
		}
		if(val_ch0>2100 && val_ch1<900 && val_ch2>=700 && val_ch3<=1400 && val_ch3>900)
		{
			printf("44444444444444444444444444444444444444！！！\r\n");
		}
		if(val_ch0<2100 && val_ch1<900 && val_ch2<200 &&  val_ch3>900 &&  val_ch3<=1600)
		{
			printf("33333333333333333333333333333333333333！！！\r\n");//左三
		}
		if(val_ch0<2100 && val_ch1<900 && val_ch2>=200 &&  val_ch2<=800 &&  val_ch3>900 &&  val_ch3<=1600)
		{
			printf("33333333333333333333333333333333333333！！！\r\n");//中三
		}

		if(val_ch0<2100 && val_ch1<900 && val_ch2>=500 && val_ch2<=700  && val_ch3>=400 && val_ch3<=900)
		{
			printf("22222222222222222222222222222222222222！！！\r\n");
		}
		if(val_ch0<2100 && val_ch1<900 && val_ch2>=400 && val_ch3<500)
		{
			printf("11111111111111111111111111111111111111！！！\r\n");
		}
		if(val_ch0<2100 && val_ch0>=1100 && val_ch1<900 && val_ch2<=200 && val_ch3<500 )
		{
			printf("拳头！！！\r\n");
		}
		
//		printf("CH0;%d CH1;%d CH2;%d CH3;%d\r\n",val_ch0,val_ch1,val_ch2,val_ch3);
		//差值
//		printf("CH0;%f CH1;%f CH2;%f CH3;%f\r\n",1000*res0,1000*res1,1000*res2,1000*res3);
		//当前值，当前得到的电容值
//		printf("CH0;%f CH1;%f CH2;%f CH3;%f\r\n",1000*temp0,1000*temp1,1000*temp2,1000*temp3);
		//固定值，为每次复位得到的初始值
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		delay_ms(1000);
	}
}

void Sys_Init(void)
{
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 //串口初始化为115200
	while(FDC2214_Init());
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
 }
 TIM5->SR&=~(1<<0);  //中断标志位清0，等待下一次中断
}
