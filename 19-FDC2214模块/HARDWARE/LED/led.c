/*
右面是led排线
从左到右
黑白灰紫蓝
c0 1 2 3 4
*/
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
void led_init()
{
  GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//GPIO C

	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;//以50mhz
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
	
}
extern void led_dark()
{
	led1=1;
	led2=1;
	led3=1;
	led4=1;
	led5=1;
}
extern void led_lignt()
{
	led1=0;
	led2=0;
	led3=0;
	led4=0;
	led5=0;
}

