#include "uart.h"
#include "adc.h"
#include "joystick.h"
#include "oled.h"
#include "menu.h"

#include <util/delay.h>
  
void main(){
    UART_Init(UBRR);
	ADC_init();
	OLED_init();

    OLED_print_arrow();
    OLED_print_arrow();

    OLED_goto_row(1);
    OLED_print_arrow();
        

}