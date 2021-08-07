/***********************************************
注意事项
一，在需要按键启动执行机构的程序中， &&关系下 按键需要放在最后一位
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
u8 a1=0,a2=0;//vs模式用的储存数据的全局变量
/************************************************
自定义延时函数
大概在0.n秒
因为在32中用定时器会造成main函数里的delay函数失效
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
数据处理
1石头
2剪刀
3布
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
	if(val_sum>=100 && val_sum<220){return 1;}//石头
	if(val_sum>=220 && val_sum<400){return 2;}//剪刀
	if(val_sum>=400)              {return 3;}//布
}
void mode_num(void);
void mode_mora(void);
void mode_pk(void);
void mode_vs(void);
void mode_learn(void);

int rand(u8 i);
/********************************
系统初始化
**********************************/
void Sys_Init()
{
	u8 i;
	delay_init();	  
	uart_init(9600);	 //串口初始化为9600
	while(FDC2214_Init());
	delay_ms(800);
	for(i=0;i<4;i++)
	{
		temp[i]=Cap_Calculate(i);//读初始值
	}
	TIM5_Ctrl_Init(500-1,7200-1);   //50ms采集一次，500/10
}
/****************************************
定时器中断服务
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
主函数
标志位m的作用：当任意按键按下的时候机器需执行对应的程序（也就是对应的模式）（反复执行才起作用）
							 如果不加标志位m，按一下机器就只会执行一次
标志位a的作用：用布尔型变量判断何时应该给m赋上新的值
							 具体位当扫描按键得到一个新的值的时候（此时按键既不在当前模式也不在悬空状态（什么也没按））
							 
标志位函数值如mode_num()等(模式运行标记)的作用：用于判断何时oled该刷新显示



标志位函数值恰好和m的作用相反
函数返回值的目的想要函数仅运行一次，之后的循环不要在运行
m的目的在于要函数一直运行除非有新的干扰或者系统有新的动作
******************************************/
int main(void)
{
	u8 key,a=1,m=8;//不相干数 标志位
	Sys_Init();//系统初始化
	led_init();//led初始化
	key_init();//按键初始化
	duoji_init();//舵机初始化
	dj_mid();
	led22=1;led33=1;led44=1;led55=1;
	point=0;
	while(1)
	{
		key = key_scan();
		if((key==1) || (m==1))//mode1猜数
		{
			led22=1;led33=1;led44=1;led55=1;
			if(key==1){dj_change();}
			mode_num();
			if(a==1){m=key;}a=0;
			if((key!=1) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
		if((key==2) || (m==2))//mode2猜拳
		{
			led22=1;led33=1;led44=1;led55=1;
			if(key==2){dj_change();}
			mode_mora();
			if(a==1){m=key;}a=0;
			if((key!=2) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
		if((key==3) || (m==3))//mode3人机
		{
			led22=1;led33=1;led44=1;led55=1;
			if(key==3){dj_change();}
			mode_pk();
			if(a==1){m=key;}a=0;
			if((key!=3) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
		if((key==4) || (m==4))//mode4人人
		{
			if(key==4){dj_change();}
			mode_vs();
			if(a==1){m=key;}a=0;
			if((key!=4) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
		if((key==5) || (m==5))//mode5训练
		{
			if(key==5){dj_change();}
			mode_learn();
			if(a==1){m=key;}a=0;
			if((key!=5) && (key!=0)){m=key;}
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//			delay_ms(1000);
		}
//		printf("CH0;%d CH1;%d CH2;%d CH3;%d\r\n",val_channel[0],val_channel[1],val_channel[2],val_channel[3]);//差值
//		printf("CH0;%d CH1;%d CH2;%d CH3;%d\r\n",(int)res[0],(int)res[1],(int)res[2],(int)res[3]);		//当前得到的电容值
//		printf("CH0;%f CH1;%f CH2;%f CH3;%f\r\n",300*temp[0],300*temp[1],300*temp[2],300*temp[3]);		//每次复位得到的初始值
	}
}
/***********************************
猜数
****************************************/
void mode_num()
{
	data_process();
	led1=0;led2=1;led3=1;led4=1;led5=1;//模式一
	if(val_sum<50)
	{
		u8 i;
//		error:
		for(i=10;i>0;i--)
		{
			dj_num();
			//横向归零
			pwm1=1;delay_us(1700);pwm1=0;delay_us(18300);
			//横向归零
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
////			printf("请重放手指!!!!!!!!!!!!!!!!!\r\n");
//			goto error;
//		}
	}
}

/************************************************
猜拳
**********************************************/
void mode_mora()
{
	u8 a;
	data_process();	
	led1=1;led2=0;led3=1;led4=1;led5=1;//模式二
	if(val_sum<50)
	{
		dj_mora();
		//横向归零
		pwm1=1;delay_us(1700);pwm1=0;delay_us(18300);
		//横向归零
		point=0;
	}
	if(val_sum>=100 && val_sum<220)
	{
//		printf("石头\r\n");
		//纵向向上加
		for(a=10;a>0;a--)
		{
			pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);		
		}
		//纵向向上加
		one();
	}
	if(val_sum>=220 &&val_sum<450)
	{
//		printf("剪刀\r\n");
		//纵向向上加
		for(a=10;a>0;a--)
		{
			pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);		
		}
		//纵向向上加
		two();
	}
	if(val_sum>=450)
	{
//		printf("布\r\n");
		//纵向向上加
		for(a=10;a>0;a--)
		{
			pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);		
		}
		//纵向向上加
		three();
	}
}
/**************************************
人机对战
放上手指机器也给出相应手势
松开机器判断输赢
***************************************/
void mode_pk()
{
	u8 i;//随机数
	u8 d;
	static u8 a;//值
	static u8 x=0;//标志位
	led1=1;led2=1;led3=0;led4=1;led5=1;//模式三
	data_process();
//	pk_start:
	if(val_sum<=100)
	{
		dj_pk();point=0;
		//横向归零
		pwm1=1;delay_us(1700);pwm1=0;delay_us(18300);
		//横向归零
		x=0;
	}
	if(val_sum>100)
	{
		delay(1);
		if(x==0)
		{
			a=rand(i)%3;//随机数012
		}
		switch(a)//根据随机数开始执行舵机操作
		{
			case 0:for(d=10;d>0;d--){pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);}three();x=1;break;
			case 1:for(d=10;d>0;d--){pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);}  one();x=1;break;
			case 2:for(d=10;d>0;d--){pwm2=1;delay_us(1200);pwm2=0;delay_us(18800);}  two();x=1;break;
		}
	}
}
/***********************************
人人
****************************************/
void mode_vs()
{
	//全局变量a1,a2储存前后的值
	static u8 b=000;//标志位
	start:
	led1=1;led2=1;led3=1;led4=0;led5=1;//模式四
	data_process();
	if(b==000 && val_sum<=100)//默认归中
	{
		dj_vs();point=0;
		led11=1;led22=1;led33=1;led44=1;led55=0;
	}
	if(b==000 && val_sum>100)//第一次检测到手
	{
		delay(5);	
		a1=data_process();
		b=001;
		led11=1;led22=1;led33=1;led44=0;led55=0;
	}
	if(b==001 && val_sum<=100)//第一次第二次空挡
	{
		delay(2);	
		b=011;
		led11=1;led22=1;led33=0;led44=0;led55=0;
	}
	if(b==011 && val_sum>100)//第二次监测到手
	{
		delay(5);	
		a2=data_process();
		b=111;
		led11=0;led22=0;led33=0;led44=0;led55=0;
	}
		//{return 1;}//石头
		//{return 2;}//剪刀
		//{return 3;}//布
	if(b==111 && (key_scan()==11))
	{
		if((a1==1 && a2==2) || (a1==2 && a2==3) || (a1==3 && a2==1) )
		{
			//前者胜！！！！！！！！！！！！！！！！
			one();b=000;
			led11=1;led22=1;led33=1;led44=1;led55=1;goto start;
		}
		if(a1==a2)
		{
			//平局！！！！！！！！！！！！！！！！！
			two();b=000;
			led11=1;led22=1;led33=1;led44=1;led55=1;goto start;
		}
		if(((a1==1) && (a2==3)) || ((a1==2) && (a2==1)) || ((a1==3) && (a2==2)))
		{
			//后者胜
			three();b=000;
			led11=1;led22=1;led33=1;led44=1;led55=1;goto start;
		}
	}
}
/*********************************
训练
*********************************/
/**************************************************************************************************************
void mode_learn()
{
	static u8 b1=000;//训练标志位
	static u8 b2=000;//空挡标志位
	led1=1;led2=1;led3=1;led4=1;led5=0;//模式五
	led11=1;led22=1;led33=1;led44=1;led55=1;
	data_process();
	if(val_sum<=120 && b1==000 && b2==000)
	{
		dj_learn();point=0;
	}
	data_process();
	if(val_sum>120 && b1==000 && b2==000 && (key_scan()==22))//第一次训练
	{
		one();b1=001;
	}
	else if(val_sum<=120 && b1==001 && b2==000)//第一次训练空挡
	{
		b2=001;
		led11=1;led22=1;led33=1;led44=1;led55=0;
	}
	if(val_sum>120 && b1==001 && b2==001 && (key_scan()==22))//第二次训练
	{
		two();b1=011;	
	}
	else if(val_sum<=120 && b1==011 && b2==001)//第er次训练空挡
	{
		b2=011;
		led11=1;led22=1;led33=1;led44=0;led55=0;
	}
	if(val_sum>120 && b1==011 && b2==011 && (key_scan()==22))//第san次训练
	{
		three();b1=111;	
	}
	else if(val_sum<=120 && b1==111 && b2==011)//第san次训练空挡
	{
		b2=111;
		led11=1;led22=1;led33=0;led44=0;led55=0;
	}
	if(val_sum<=120 && b1==111 && b2==111 && (key_scan()==22))//训练结束
	{
		//激光笔闪烁
		//三组
		led11=1;led22=1;led33=1;led44=1;led55=1;
		point=0;delay(1);point=1;delay(1);
		point=0;delay(1);point=1;delay(1);
		point=0;delay(1);point=1;delay(1);
		b1=000;b2=000;
	}
}
************************************************************************************************************/
/*********************************
训练
*********************************/
void mode_learn()
{
	static u8 b1=000;//训练标志位
	start:
	led1=1;led2=1;led3=1;led4=1;led5=0;//模式五
	led11=1;led22=1;led33=1;led44=1;led55=1;
	data_process();
	if(val_sum<=80 && b1==000)
	{
		dj_learn();point=0;
	}
	if(b1==000 && (key_scan()==22))//第一次训练
	{
		one();b1=001;
	}
	else if(b1==001 && (key_scan()==22))//第二次训练
	{
		two();b1=011;	
	}
	else if(b1==011 && (key_scan()==22))//第san次训练
	{
		three();b1=111;	
	}
	if(b1==111 && (key_scan()==22))//训练结束
	{
		//激光笔闪烁
		//三组
		point=0;delay(1);point=1;delay(1);
		point=0;delay(1);point=1;delay(1);
		point=0;delay(1);point=1;delay(1);
		b1=000;goto start;
	}
}
