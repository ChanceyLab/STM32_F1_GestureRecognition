#include "stm32f10x.h"
#ifndef __OLED_H
#define __OLED_H 

//对相应引脚进行宏定义，方便使用
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_15)//CLK、D0
#define OLED_SCLK_Set() GPIO_SetBits(GPIOG,GPIO_Pin_15)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)//MOSI、D1
#define OLED_SDIN_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_11)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOG,GPIO_Pin_11)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_9)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOG,GPIO_Pin_9)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_12)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_12)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_Init(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowString(u8 x,u8 y,char *chr);
//void OLED_ShowString(u8 x,u8 y,u8 *chr);
void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N); 
void OLED_56x56CN(unsigned char x, unsigned char y, unsigned char N);
void OLED_16x16CNStr(unsigned char x, unsigned char y, unsigned char *str);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);



void caishu(void);
void yi(void);
void er(void);
void san(void);
void si(void);
void wu(void);
void caiquan(void);
void shi(void);
void dao(void);
void bu(void);
void warn1(void);
void warn2(void);
void win(void);
void lose(void);
void ping(void);
void pk(void);
#endif
