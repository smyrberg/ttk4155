#if 0


#include "drivers/pwm.h"
#include <avr/io.h>
#include <math.h>

int main()
{
	PWM_init();
	
	double angle = 90.0;
	while(1)
	{
		double input = 1+sin(angle);
		SERVO_set_pos(input);
		angle += 0.0001;
	}	
}

#endif