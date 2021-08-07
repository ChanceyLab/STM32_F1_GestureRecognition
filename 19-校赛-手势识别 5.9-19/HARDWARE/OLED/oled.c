#include "stm32f10x.h"
#include "oled.h"
#include "oledfount.h"
#include "delay.h"

u8 OLED_GRAM[128][8];


void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	  OLED_CS_Clr();
	for(i=0;i<8;i++)
	{		  
		   OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		
		   OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 


void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);//设置页面开始 页面地址寻址模式
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);//设置低的列开始地址页面寻址模式
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); //设置列地址
} 

   

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}


//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	


//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


//初始化SSD1306					    
void OLED_Init(void)
{ 	
 	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	      //使能G端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;     
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		        //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //速度50MHz
 	GPIO_Init(GPIOG, &GPIO_InitStructure);	                    //初始化GPIOA12,15
 	GPIO_SetBits(GPIOG,GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15);
	
	
	
 
	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  



/*******************************************************************************
  函数名：void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N)
  描述  ：在OLED上显示16x16中文汉字,汉字要先在取模软件中取模
  输入  ：x表示oled横坐标：0-110
          y表示oled纵坐标：0-6
          N表示为数组里面的所要表示的字在第几位。
  输出  ：无
  调用方法 ：只能一个字一个字的打印，N表示为数组里面的所要表示的字在第几位。
 *****************************************************************************/
void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WR_Byte(F16x16[adder],OLED_DATA);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WR_Byte(F16x16[adder],OLED_DATA);
		adder += 1;
	}
}

void OLED_56x56CN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=392*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WR_Byte(F56x56[adder],OLED_DATA);
		adder += 1;
	}                      
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WR_Byte(F56x56[adder],OLED_DATA);
		adder += 1;
	}
		OLED_Set_Pos(x,y + 2);
	for(wm = 0;wm < 56;wm++)
	{
		OLED_WR_Byte(F56x56[adder],OLED_DATA);
		adder += 1;
	}
		OLED_Set_Pos(x,y + 3);
	for(wm = 0;wm < 56;wm++)
	{
		OLED_WR_Byte(F56x56[adder],OLED_DATA);
		adder += 1;
	}
		OLED_Set_Pos(x,y + 4);
	for(wm = 0;wm < 56;wm++)
	{
		OLED_WR_Byte(F56x56[adder],OLED_DATA);
		adder += 1;
	}
		OLED_Set_Pos(x,y + 5);
	for(wm = 0;wm < 56;wm++)
	{
		OLED_WR_Byte(F56x56[adder],OLED_DATA);
		adder += 1;
	}
		OLED_Set_Pos(x,y + 6);
	for(wm = 0;wm < 56;wm++)
	{
		OLED_WR_Byte(F56x56[adder],OLED_DATA);
		adder += 1;
	}
}

/*******************************************************
显示各种内容
*******************************************************/
void caishu()//猜数模式
{
	OLED_16x16CN(0,0,14);	
	OLED_16x16CN(32,0,15);
	OLED_16x16CN(64,0,16);
	OLED_16x16CN(96,0,17);
}

void yi()
{
		OLED_16x16CN(0,0,22);
		OLED_16x16CN(32,0,23);
		OLED_16x16CN(64,0,24);
		OLED_16x16CN(96,0,0);
}
void er()
{
		OLED_16x16CN(0,0,22);
		OLED_16x16CN(32,0,23);
		OLED_16x16CN(64,0,24);
		OLED_16x16CN(96,0,1);
}
void san()
{
		OLED_16x16CN(0,0,22);
		OLED_16x16CN(32,0,23);
		OLED_16x16CN(64,0,24);
		OLED_16x16CN(96,0,2);
}
void si()
{
		OLED_16x16CN(0,0,22);
		OLED_16x16CN(32,0,23);
		OLED_16x16CN(64,0,24);
		OLED_16x16CN(96,0,3);
}
void wu()
{
		OLED_16x16CN(0,0,22);
		OLED_16x16CN(32,0,23);
		OLED_16x16CN(64,0,24);
		OLED_16x16CN(96,0,4);
}
void caiquan()
{
	OLED_16x16CN(0,0,14);	
	OLED_16x16CN(32,0,18);
	OLED_16x16CN(64,0,16);
	OLED_16x16CN(96,0,17);
}
void shi()
{
		OLED_16x16CN(0,0,22);
		OLED_16x16CN(32,0,23);
		OLED_16x16CN(64,0,24);
	OLED_16x16CN(96,0,18);
}
void dao()
{
		OLED_16x16CN(0,0,22);
		OLED_16x16CN(32,0,23);
		OLED_16x16CN(64,0,24);
	OLED_16x16CN(96,0,8);
}
void bu()
{
		OLED_16x16CN(0,0,22);
		OLED_16x16CN(32,0,23);
		OLED_16x16CN(64,0,24);
		OLED_16x16CN(96,0,9);
}
void warn1()//请重放手指
{
		OLED_16x16CN(0,0,25);
		OLED_16x16CN(17,0,26);
		OLED_16x16CN(33,0,27);
		OLED_16x16CN(49,0,22);
		OLED_16x16CN(65,0,28);
}
void warn2()//请复位
{
		OLED_16x16CN(0,0,25);
		OLED_16x16CN(32,0,29);
		OLED_16x16CN(64,0,30);
}
void win()
{
		OLED_16x16CN(0,0,31);
		OLED_16x16CN(32,0,32);
		OLED_16x16CN(64,0,34);
}
void lose()
{
		OLED_16x16CN(0,0,31);
		OLED_16x16CN(32,0,33);
		OLED_16x16CN(64,0,34);
}
void ping()
{
		OLED_16x16CN(0,0,35);
		OLED_16x16CN(32,0,36);
		OLED_16x16CN(64,0,34);
}
void pk()
{
		OLED_16x16CN(0,0,37);
		OLED_16x16CN(17,0,38);
		OLED_16x16CN(33,0,39);
		OLED_16x16CN(49,0,40);
		OLED_16x16CN(65,0,16);
		OLED_16x16CN(81,0,17);
}






