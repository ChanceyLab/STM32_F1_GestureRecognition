/*89
左面是key排线
从左到右
棕红橙黄绿
c6 7 8 9 10
*/
/*
黄-绿
lan-hong
a146b02
*/
#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#define k1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define k2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define k3 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)
#define k4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)
#define k5 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)
#define k11 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define k22 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define k33 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define k44 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define k55 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
//一端接地的按键，默认情况下为不按下，所以设置为上拉输入
//接vcc则反之
void key_init()
{
  GPIO_InitTypeDef GPIO_Initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//C	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
//A	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6);
//B
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_0);
	
}
u8 key_scan()             
{
	static u8 k_puss = 1;
	u8 a;
	if(k_puss && (k1==0||k2==0||k3==0||k4==0||k5==0||k11==0||k22==0||k33==0||k44==0||k55==0))
	{
		delay_ms(10);
		if(k1==0)
		{
			k_puss = 0;a= 1;
		}
		if(k2==0)
		{
			k_puss = 0;a= 2;
		}
		if(k3==0)
		{
			k_puss = 0;a= 3;
		}
		if(k4==0)
		{
			k_puss = 0;a= 4;
		}
		if(k5==0)
		{
			k_puss = 0;a= 5;
		}
		if(k11==0)
		{
			k_puss = 0;a= 11;
		}
		if(k22==0)
		{
			k_puss = 0;a= 22;
		}
		if(k33==0)
		{
			k_puss = 0;a= 33;
		}
		if(k44==0)
		{
			k_puss = 0;a= 44;
		}
		if(k55==0)
		{
			k_puss = 0;a= 55;
		}
	}		
	else if((k_puss==1) || (k1==1 && k2==1 && k3==1 && k4==1 && k5==1&& k11==1 && k22==1 && k33==1 && k44==1 && k55==1))
	{
		k_puss = 1;return 0;
	}
	return a;
}


