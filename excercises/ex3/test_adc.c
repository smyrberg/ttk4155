#include "adc.h"
#include "../../drivers/uart.h"

#include <util/delay.h>


int main(){
    ADC_init();
    UART_Init(UBRR);

    uint8_t x, y, slider_left, slider_right;
    while(1){
        x = ADC_read(0);
        y = ADC_read(1);
        slider_left = ADC_read(2);
        slider_right = ADC_read(3);
        printf("X: %3d\tY: %3d\tSlider Left: %3d\tSlider Right: %3d\r\n", x, y, slider_left, slider_right);
        _delay_ms(100);
    }   
}