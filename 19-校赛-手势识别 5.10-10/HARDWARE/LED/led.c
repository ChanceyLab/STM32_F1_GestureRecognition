/*
右面是led排线
从左到右
黑白灰紫蓝
c0 1 2 3 4
*/
/*
f11-a0
lan-zong
*/
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
extern void led_init()
{
  GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//GPIO C
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIO A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIO B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//GPIO F
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//GPIO G
//C
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;//以50mhz
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
//A	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;//以50mhz
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_5 | GPIO_Pin_7);
//B	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_9;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;//以50mhz
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_9);
//F	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;//以50mhz
	GPIO_Init(GPIOF,&GPIO_Initstructure);
	GPIO_SetBits(GPIOF,GPIO_Pin_11);
//G	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_14;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;//以50mhz
	GPIO_Init(GPIOG,&GPIO_Initstructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_1 | GPIO_Pin_14);
}
