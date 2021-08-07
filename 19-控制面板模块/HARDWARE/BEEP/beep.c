#include "stm32f10x.h"
#include "beep.h"
void beep_init()
{
	GPIO_InitTypeDef GPIO_Initstructure;//初始化选择形式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIOB
	//三种初始化要素
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_Init(GPIOB,&GPIO_Initstructure);

}



