#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "delay.h"
//void welcome()
//{
//			led1 = 0;
//			delay_ms(500);
//			led_init();
//
//			led2 = 0;
//			delay_ms(500);
//			led_init();
//
//			led3 = 0;
//			delay_ms(500);
//			led_init();
//
//			led4 = 0;
//			delay_ms(500);
//			led_init();
//
//			led5 = 0;
//			delay_ms(500);
//			led_init();
//}
int main()
{
    u8 key_val;
    led_init();
    key_init();
    delay_init();
    while(1)
    {
        key_val = key_scan();
        switch (key_val)
        {
        case (1):
            led_dark();
            led1 = 0;
            break;
        case (2):
            led_dark();
            led2 = 0;
            break;
        case (3):
            led_dark();
            led3 = 0;
            break;
        case (4):
            led_dark();
            led4 = 0;
            break;
        case (5):
            led_dark();
            led5 = 0;
            break;
        }
        delay_ms(20);
    }
}