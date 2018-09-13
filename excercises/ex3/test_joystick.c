#include "joystick.h"
#include "uart.h"

#include <util/delay.h>


int main(){
     JOY_init();
     UART_Init(UBRR);

    JOY_position_t pos;
    
    while(1){
        pos = JOY_get_position();
        printf("x: %d  y:%d \r\n", pos.x, pos.y);
        _delay_ms(500);

    }
}