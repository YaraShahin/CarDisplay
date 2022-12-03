#include <avr/io.h>
#include <util/delay.h>

void pwm_init(unsigned char pin,unsigned char reg);
void set_dutyCycle(int pin, int duty);