#include "duoji.h"
#include "stm32f10x.h"
#include "delay.h"
int a;
void duoji_init()
{
  GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//GPIO E
	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;//��50mhz
	GPIO_Init(GPIOE,&GPIO_Initstructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1);
}
extern void dj_mid()//����
{
	for(a=20;a>0;a--)	
	{
		pwm2=1;
		delay_us(1550);
		pwm2=0;
		delay_us(14500);
	}
	for(a=20;a>0;a--)	
	{
		pwm1=1;
		delay_us(1350);
		pwm1=0;
		delay_us(16500);
	}
}
extern void dj_change()//ģʽѡ��
{
	for(a=30;a>0;a--)	
	{
		pwm1=1;
		delay_us(1700);
		pwm1=0;
		delay_us(18300);
	}
}
extern void dj_mora()//��ȭģʽ
{
		pwm2=1;
		delay_us(1200);
		pwm2=0;
		delay_us(18800);
}
extern void dj_pk()//�˻���ս
{
		pwm2=1;
		delay_us(1340);
		pwm2=0;
		delay_us(18660);
}
extern void dj_vs()//���˶�ս
{
		pwm2=1;
		delay_us(1480);
		pwm2=0;
		delay_us(18520);
}
extern void dj_learn()//ѵ��
{
		pwm2=1;
		delay_us(1650);
		pwm2=0;
		delay_us(18350);
}
extern void dj_num()//����ģʽ
{
		pwm2=1;
		delay_us(1820);
		pwm2=0;
		delay_us(18180);
}
extern void dj_num_1()//����ģʽ��һ��
{
	for(a=20;a>0;a--)
	{
		pwm2=1;
		delay_us(1800);
		pwm2=0;
		delay_us(18200);
	}
}
extern void dj_num_2()//����ģʽ�ڶ���
{
	for(a=20;a>0;a--)
	{
		pwm2=1;
		delay_us(1940);
		pwm2=0;
		delay_us(18060);
	}
}

extern void three()//������
{
	for(a=30;a>0;a--)
	{
		pwm1=1;
		delay_us(1030);
		pwm1=0;
		delay_us(18970);
	}
}
extern void one()//��һ��
{
	for(a=25;a>0;a--)
	{
		pwm1=1;
		delay_us(1550);
		pwm1=0;
		delay_us(18450);
	}
}
extern void two()//�ڶ���
{
	for(a=20;a>0;a--)
	{
		pwm1=1;
		delay_us(1320);
		pwm1=0;
		delay_us(18680);
	}
}
