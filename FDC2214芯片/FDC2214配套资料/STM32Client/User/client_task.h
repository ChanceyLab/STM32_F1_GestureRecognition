#ifndef __CLIENT_TASK_
#define __CLIENT_TASK_

typedef enum
{
	BOXDisable =0,//失能所设置项
	BOXEnable =1,//使能所设置项
}Enable_bit;

typedef enum
{  //FDC单通道选择 仅仅在自动扫描失能时有效
  Chan0 = 0x01,//选择 chan0  
  Chan1 = 0x02,//选择 chan1  
  Chan2 = 0x03,//选择 chan2  
  Chan3 = 0x04,//选择 chan3  
}Single_chan;

typedef enum
{  //自动扫描模式使能后，FDC多通道选择
	Multi_2 =0x01,      //选用ch0 ch1 
	Multi_3 =0x02,      //选用ch0 ch1 ch2
	Multi_4 =0x03,      //选用ch0 ch1 ch2 ch3
}Multi_chan;

typedef enum
{  //时钟源
	Clk_src_external  =0x01,//选择外部时钟 传感器板上焊有40MHz晶振时选择
	Clk_src_internal  =0x02,//选择内部时钟  
}Clk_src;

typedef enum
{  //抗尖峰脉冲滤波器带宽
  Bandwidth_1M		=0x01, //1MHz	 设置带宽为1MHz
  Bandwidth_3D3M	=0x02, //3.3MHz	 带宽设置需要大于传感器各通道振荡频率中的最大值（创阿奇）
  Bandwidth_10M	  	=0x03, //10MHz
  Bandwidth_33M		=0x04, //33MHz
}Bandwidth;

typedef enum
{
	ref_count_ch0 = 0x01,//参考计数功能号，对应传感器参考计数值。参考计数值范围：0x0100-0xFFFF 
	ref_count_ch1 = 0x02,//传感器各通道的转换时间为：（参考计数值*16）/ fREF(参考频率值)
	ref_count_ch2 = 0x03,
	ref_count_ch3 = 0x04,
	
	settle_count_ch0= 0x05,//稳定计数功能号，对应各通道稳定计数值。稳定计数值范围：0x0002 - 0xFFFF
	settle_count_ch1= 0x06,//各通道的稳定时间 = （稳定计数值*16）/ fREF
	settle_count_ch2= 0x07,//当稳定计数值为0x0000或0x0001时，各通道稳定时间为：32/fREF
	settle_count_ch3= 0x08,
	
	drive_current_ch0= 0x09,//驱动电流 0~31:用来驱动对应传感器通道的电流
	drive_current_ch1= 0x0a,//
	drive_current_ch2= 0x0b,
	drive_current_ch3= 0x0c,
	
	fref_dividers_ch0= 0x0d,//参考频率分频系数功能号，对应参考频率分频系数值。参考频率分频系数值范围：1~1023
	fref_dividers_ch1= 0x0e,//参考频率分频系数用来对相应的通道参考频率进行分频：				
	fref_dividers_ch2= 0x0f,//各通道参考频率 =  时钟频率/对应通道参考频率分频系数
	fref_dividers_ch3= 0x10,
	
	fin_dividers_ch0= 0x11,//输入分频系数功能号，对应各通道输入信号分频系数。输入信号分频系数取值范围：1或2
	fin_dividers_ch1= 0x12,//当传感器各通道输入频率在0.01MHz到8.75MHz之间时，对应通道输入信号分频系数需要设置为 1 
	fin_dividers_ch2= 0x13,//当传感器各通道输入频率在5MHz到10MHz之间时，     对应通道输入信号分频系数需要设置为 2 
	fin_dividers_ch3= 0x14,//当传感器各通道输入频率在0.01MHz到10MHz之间时，  对应通道输入信号分频系数可以设置为 2 
	
	autoscan= 0x15,//自动扫描功能号，对应自动扫描使能位。自动扫描使能位取值范围见上面的 Enable_bit 枚举说明
	autoscan_ch= 0x16,//自动扫描通道功能号，对应自动扫描模式下工作的通道。自动扫描模式下工作的通道取值范围见上面的 Multi_chan 枚举说明
	single_ch= 0x17,//单通道选择功能号，对应单通道模式下选用的通道。单通道模式下选用的通道取值范围见上面的 Single_chan 枚举说明
	
	clk_src= 0x18,//时钟源设置功能号，对应时钟源选择。时钟源选择取值范围见上面的 Clk_src 枚举说明
	deglitch_bandwidth= 0x19,//抗尖峰脉冲滤波器带宽，其对应的取值范围见上面的 Bandwidth 枚举说明
	activate_sensor= 0x1a,//激活传感器选项，对应传感器激活使能位。传感器激活使能取值范围见上面的 Enable_bit 枚举说明
						  //当使能激活传感器位时，传感器会以高电流激活传感器以减小激活时间，否则会用设置的驱动电流来激活传感器以减少功耗
	high_current_drive= 0x1b,//高电流驱动功能号，对应高电流驱动使能位。高电流驱动使能位取值范围见上面的 Enable_bit 枚举说明
							 //高电流驱动只在单通道模式下有效！当使能高电流驱动时，传感器会以超过1.5mA的电流驱动通道0；否则，会以设置的驱动电流驱动通道0
	
	reset_device= 0x1c,//复位设备功能号，对应设备复位选项。设备复位选项取值只能为BOXDisable，当使用此功能号后，会复位FDC2214寄存器为出厂时的配置
	sleep= 0x1d,//睡眠模式功能号，对应睡眠模式使能位。睡眠模式使能取值范围见上面的 Enable_bit 枚举说明
				//当使能睡眠模式使能位后，FDC2214芯片进入睡眠模式，停止工作，不再输出数据
}ClientID;

#endif
