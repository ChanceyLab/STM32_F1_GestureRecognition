  /*
左面是key排线
从左到右
棕红橙黄绿
c6 7 8 9 10
*/
#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#define k1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define k2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define k3 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)
#define k4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)
#define k5 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)
//一端接地的按键，默认情况下为不按下，所以设置为上拉输入
//接vcc则反之
void key_init()
{
  GPIO_InitTypeDef GPIO_Initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
}
u8 key_scan()             
{
	static u8 k_puss = 1;
	if(k_puss && (k1==0||k2==0||k3==0||k4==0||k5==0))
	{
		delay_ms(20);
		if(k1==0)
		{
			k_puss = 0;
			return 1;
		}
		if(k2==0)
		{
			k_puss = 0;
			return 2;
		}
		if(k3==0)
		{
			k_puss = 0;
			return 3;
		}
		if(k4==0)
		{
			k_puss = 0;
			return 4;
		}
		if(k5==0)
		{
			k_puss = 0;
			return 5;
		}
	}		
	else if((k_puss==1) || (k1==1 && k2==1 && k3==1 && k4==1 && k5==1))
	{
		k_puss = 1;
		return 0;
	}

}


