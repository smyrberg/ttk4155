#include "../../../common/bitmanip.h"

#include <avr/io.h>


void enable_ext_mem(void){
	// enable ext mem
	set_bit(MCUCR, SRE);
}

void release_jtag(){
	// release PC7-PC4 for JTAG
	set_bit(SFIOR, XMM2);
}
