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

    while(1){

        OLED_print_arrow();
        _delay_ms(1000);
        
        for(int i = 0; i < 127*9; i++){
    		OLED_write_data(0x0);
    	}
    }


}