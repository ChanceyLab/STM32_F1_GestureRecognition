/***********************************************
ע������
һ������Ҫ��������ִ�л����ĳ����У� &&��ϵ�� ������Ҫ�������һλ
***********************************************/
#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "duoji.h"
#include "led.h"
#include "key.h"
#include "fdc2214.h"
#include "IIC.h"
#include "stdio.h"
int val_channel[]={0,0,0,0};
float temp[]={0,0,0,0};
float res[]={0,0,0,0};
int val_sum;
u8 a1=0,a2=0;//vsģʽ�õĴ������ݵ�ȫ�ֱ���
/************************************************
�Զ�����ʱ����
�����0.n��
��Ϊ��32���ö�ʱ�������main�������delay����ʧЧ
************************************************/
void delay(u8 n)   //?? -0.363636363587us
{
  u8 a,b,c;
	for(;n>0;n--)
    for(c=41;c>0;c--)
        for(b=190;b>0;b--)
            for(a=175;a>0;a--);
}
/************************************************
���ݴ���
1ʯͷ
2����
3��
***********************************************/
u8 data_process()
{
	u8 i;
	for(i=0;i<4;i++)
	{
		val_channel[i]=(res[i]-temp[i])*300;
		val_channel[i]-=val_channel[i]%10;
	}
	val_sum=val_channel[0]+val_channel[1]+val_channel[2]+val_channel[3];
	if(val_sum>=100 && val_sum<220){return 1;}//ʯͷ
	if(val_sum>=220 && val_sum<400){return 2;}//����
	if(val_sum>=400)              {return 3;}//��
}
void mode_num(void);
void mode_mora(void);
void mode_pk(void);
void mode_vs(void);
void mode_learn(void);

int rand(u8 i);
/********************************
ϵͳ��ʼ��
**********************************/
void Sys_Init()
{
	u8 i;
	delay_init();	  
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	while(FDC2214_Init());
	delay_ms(800);
	for(i=0;i<4;i++)
	{
		temp[i]=Cap_Calculate(i);//����ʼֵ
	}
	TIM5_Ctrl_Init(500-1,7200-1);   //50ms�ɼ�һ�Σ�500/10
}
/****************************************
��ʱ���жϷ���
****************************************/
void TIM5_IRQHandler()
{ 
	if(TIM5->SR&0X0001)
	{
		u8 i;
		for(i=0;i<4;i++)
		{
			res[i]=Cap_Calculate(i);
		}
	}
	TIM5->SR&=~(1<<0); 
}
/***************************************
������
��־λm�����ã������ⰴ�����µ�ʱ�������ִ�ж�Ӧ�ĳ���Ҳ���Ƕ�Ӧ��ģʽ��������ִ�в������ã�
							 ������ӱ�־λm����һ�»�����ֻ��ִ��һ��
��־λa�����ã��ò����ͱ����жϺ�ʱӦ�ø�m�����µ�ֵ
							 ����λ��ɨ�谴���õ�һ���µ�ֵ��ʱ�򣨴�ʱ�����Ȳ��ڵ�ǰģʽҲ��������״̬��ʲôҲû������
							 
��־λ����ֵ��mode_num()��(ģʽ���б��)�����ã������жϺ�ʱoled��ˢ����ʾ



��־λ����ֵǡ�ú�m�������෴
��������ֵ��Ŀ����Ҫ����������һ�Σ�֮���ѭ����Ҫ������
m��Ŀ������Ҫ����һֱ���г������µĸ��Ż���ϵͳ���µĶ���
******************************************/
int main(void)
{
	u8 key,a=1,m=8;//������� ��־λ
	Sys_Init();//ϵͳ��ʼ��
	led_init();//led��ʼ��
	key_init();//������ʼ��
	duoji_init();//�����ʼ��
	dj_mid();
	led22=1;led33=1;led44=1;led55=1;
	point=0;
	while(1)
	{
		key = key_scan();
		if((key==1) || (m==1))//mode1����
		{
			led22=1;led33=1;led44=1;led55=1;
			if(key==1){dj_change();}
			mode_num();
			if(a==1){m=key;}a=0;
			if((key!=1) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
		if((key==2) || (m==2))//mode2��ȭ
		{
			led22=1;led33=1;led44=1;led55=1;
			if(key==2){dj_change();}
			mode_mora();
			if(a==1){m=key;}a=0;
			if((key!=2) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
		if((key==3) || (m==3))//mode3�˻�
		{
			led22=1;led33=1;led44=1;led55=1;
			if(key==3){dj_change();}
			mode_pk();
			if(a==1){m=key;}a=0;
			if((key!=3) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
		if((key==4) || (m==4))//mode4����
		{
			if(key==4){dj_change();}
			mode_vs();
			if(a==1){m=key;}a=0;
			if((key!=4) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
		if((key==5) || (m==5))//mode5ѵ��
		{
			if(key==5){dj_change();}
			mode_learn();
			if(a==1){m=key;}a=0;
			if((key!=5) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
//		printf("CH0;%d CH1;%d CH2;%d CH3;%d\r\n",val_channel[0],val_channel[1],val_channel[2],val_channel[3]);//��ֵ
//		printf("CH0;%d CH1;%d CH2;%d CH3;%d\r\n",(int)res[0],(int)res[1],(int)res[2],(int)res[3]);		//��ǰ�õ��ĵ���ֵ
//		printf("CH0;%f CH1;%f CH2;%f CH3;%f\r\n",300*temp[0],300*temp[1],300*temp[2],300*temp[3]);		//ÿ�θ�λ�õ��ĳ�ʼֵ
	}
}
/***********************************
����
****************************************/
void mode_num()
{
	data_process();
	led1=0;led2=1;led3=1;led4=1;led5=1;//ģʽһ
	if(val_sum<50)
	{
		u8 i;
//		error:
		for(i=10;i>0;i--)
		{
			dj_num();
			//�������
			pwm1=1;delay_us(1700);pwm1=0;delay_us(18300);
			//�������
			point=0;
		}
	}
	if(val_sum>50)
	{
		if(val_channel[2]>=170 && (val_channel[0]+val_channel[1]+val_channel[3])<50)
		{
//			printf("111111111111111111111111111\r\n");
			dj_num_1();one();
		}
		else if(val_channel[2]>=170 && val_channel[1]>50 && (val_channel[0]+val_channel[3])<50)
		{
//			printf("222222222222222222222222222\r\n");
			dj_num_1();two();
		}
		else if(val_channel[2]<=170 && val_channel[1]>50 && val_channel[0] >100 && val_channel[3]<50)
		{
//			printf("333333333333333333333333333\r\n");
			dj_num_1();three();
		}
		else if(val_channel[2]>170 && val_channel[1]>50 && val_channel[0] >100 && val_channel[3]<50)
		{
//			printf("444444444444444444444444444\r\n");
			dj_num_2();one();
		}
		else if(val_sum>400 && val_channel[3]>=50)
		{
//			printf("555555555555555555555555555\r\n");
			dj_num_2();three();
		}
//		else
//		{
////			printf("���ط���ָ!!!!!!!!!!!!!!!!!\r\n");
//			goto error;
//		}
	}
}

/************************************************
��ȭ
**********************************************/
void mode_mora()
{
	u8 a;
	data_process();	
	led1=1;led2=0;led3=1;led4=1;led5=1;//ģʽ��
	if(val_sum<50)
	{
		dj_mora();
		//�������
		pwm1=1;delay_us(1700);pwm1=0;delay_us(18300);
		//�������
		point=0;
	}
	if(val_sum>=100 && val_sum<220)
	{
//		printf("ʯͷ\r\n");
		//�������ϼ�
		for(a=10;a>0;a--)
		{
			pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);		
		}
		//�������ϼ�
		one();
	}
	if(val_sum>=220 &&val_sum<450)
	{
//		printf("����\r\n");
		//�������ϼ�
		for(a=10;a>0;a--)
		{
			pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);		
		}
		//�������ϼ�
		two();
	}
	if(val_sum>=450)
	{
//		printf("��\r\n");
		//�������ϼ�
		for(a=10;a>0;a--)
		{
			pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);		
		}
		//�������ϼ�
		three();
	}
}
/**************************************
�˻���ս
������ָ����Ҳ������Ӧ����
�ɿ������ж���Ӯ
***************************************/
void mode_pk()
{
	u8 i;//�����
	u8 d;
	static u8 a;//ֵ
	static u8 x=0;//��־λ
	led1=1;led2=1;led3=0;led4=1;led5=1;//ģʽ��
	data_process();
//	pk_start:
	if(val_sum<=100)
	{
		dj_pk();point=0;
		//�������
		pwm1=1;delay_us(1700);pwm1=0;delay_us(18300);
		//�������
		x=0;
	}
	if(val_sum>100)
	{
		delay(1);
		if(x==0)
		{
			a=rand(i)%3;//�����012
		}
		switch(a)//�����������ʼִ�ж������
		{
			case 0:for(d=10;d>0;d--){pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);}three();x=1;break;
			case 1:for(d=10;d>0;d--){pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);}  one();x=1;break;
			case 2:for(d=10;d>0;d--){pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);}  two();x=1;break;
		}
	}
}
/***********************************
����
****************************************/
void mode_vs()
{
	//ȫ�ֱ���a1,a2����ǰ���ֵ
	static u8 b=000;//��־λ
	start:
	led1=1;led2=1;led3=1;led4=0;led5=1;//ģʽ��
	data_process();
	if(b==000 && val_sum<=100)//Ĭ�Ϲ���
	{
		dj_vs();point=0;
		led11=1;led22=1;led33=1;led44=1;led55=0;
	}
	if(b==000 && val_sum>100)//��һ�μ�⵽��
	{
		delay(5);	
		a1=data_process();
		b=001;
		led11=1;led22=1;led33=1;led44=0;led55=0;
	}
	if(b==001 && val_sum<=100)//��һ�εڶ��οյ�
	{
		delay(2);	
		b=011;
		led11=1;led22=1;led33=0;led44=0;led55=0;
	}
	if(b==011 && val_sum>100)//�ڶ��μ�⵽��
	{
		delay(5);	
		a2=data_process();
		b=111;
		led11=0;led22=0;led33=0;led44=0;led55=0;
	}
		//{return 1;}//ʯͷ
		//{return 2;}//����
		//{return 3;}//��
	if(b==111 && (key_scan()==11))
	{
		if((a1==1 && a2==2) || (a1==2 && a2==3) || (a1==3 && a2==1) )
		{
			//ǰ��ʤ��������������������������������
			one();b=000;
			led11=1;led22=1;led33=1;led44=1;led55=1;goto start;
		}
		if(a1==a2)
		{
			//ƽ�֣���������������������������������
			two();b=000;
			led11=1;led22=1;led33=1;led44=1;led55=1;goto start;
		}
		if(((a1==1) && (a2==3)) || ((a1==2) && (a2==1)) || ((a1==3) && (a2==2)))
		{
			//����ʤ
			three();b=000;
			led11=1;led22=1;led33=1;led44=1;led55=1;goto start;
		}
	}
}
/*********************************
ѵ��
*********************************/
/**************************************************************************************************************
void mode_learn()
{
	static u8 b1=000;//ѵ����־λ
	static u8 b2=000;//�յ���־λ
	led1=1;led2=1;led3=1;led4=1;led5=0;//ģʽ��
	led11=1;led22=1;led33=1;led44=1;led55=1;
	data_process();
	if(val_sum<=120 && b1==000 && b2==000)
	{
		dj_learn();point=0;
	}
	data_process();
	if(val_sum>120 && b1==000 && b2==000 && (key_scan()==22))//��һ��ѵ��
	{
		one();b1=001;
	}
	else if(val_sum<=120 && b1==001 && b2==000)//��һ��ѵ���յ�
	{
		b2=001;
		led11=1;led22=1;led33=1;led44=1;led55=0;
	}
	if(val_sum>120 && b1==001 && b2==001 && (key_scan()==22))//�ڶ���ѵ��
	{
		two();b1=011;	
	}
	else if(val_sum<=120 && b1==011 && b2==001)//��er��ѵ���յ�
	{
		b2=011;
		led11=1;led22=1;led33=1;led44=0;led55=0;
	}
	if(val_sum>120 && b1==011 && b2==011 && (key_scan()==22))//��san��ѵ��
	{
		three();b1=111;	
	}
	else if(val_sum<=120 && b1==111 && b2==011)//��san��ѵ���յ�
	{
		b2=111;
		led11=1;led22=1;led33=0;led44=0;led55=0;
	}
	if(val_sum<=120 && b1==111 && b2==111 && (key_scan()==22))//ѵ������
	{
		//�������˸
		//����
		led11=1;led22=1;led33=1;led44=1;led55=1;
		point=0;delay(1);point=1;delay(1);
		point=0;delay(1);point=1;delay(1);
		point=0;delay(1);point=1;delay(1);
		b1=000;b2=000;
	}
}
************************************************************************************************************/
/*********************************
ѵ��
*********************************/
void mode_learn()
{
	static u8 b1=000;//ѵ����־λ
	start:
	led1=1;led2=1;led3=1;led4=1;led5=0;//ģʽ��
	led11=1;led22=1;led33=1;led44=1;led55=1;
	data_process();
	if(val_sum<=80 && b1==000)
	{
		dj_learn();point=0;
	}
	if(b1==000 && (key_scan()==22))//��һ��ѵ��
	{
		one();b1=001;
	}
	else if(b1==001 && (key_scan()==22))//�ڶ���ѵ��
	{
		two();b1=011;	
	}
	else if(b1==011 && (key_scan()==22))//��san��ѵ��
	{
		three();b1=111;	
	}
	if(b1==111 && (key_scan()==22))//ѵ������
	{
		//�������˸
		//����
		point=0;delay(1);point=1;delay(1);
		point=0;delay(1);point=1;delay(1);
		point=0;delay(1);point=1;delay(1);
		b1=000;goto start;
	}
}
