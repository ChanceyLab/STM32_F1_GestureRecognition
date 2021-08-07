#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"

#endif


#define led1 PCout(0)
#define led2 PCout(1)
#define led3 PCout(2)
#define led4 PCout(3)
#define led5 PCout(4)
#define led11 PGout(14)
#define led22 PBout(3)
#define led33 PBout(5)
#define led44 PBout(7)
#define led55 PBout(9)
#define point PGout(1)

extern void led_init(void);
