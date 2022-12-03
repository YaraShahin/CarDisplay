#include <avr/io.h>
#include <util/delay.h>

void pwm_init(unsigned char pin,unsigned char reg){
   if (pin==11 or pin==3){
      TCCR2B = TCCR2B & ~(1<<1);
      TCCR2A = TCCR2A | (11<<0);

      TCCR2B = TCCR2B | (1<<0);
      TCCR2B = TCCR2B & ~(1<<1);
      TCCR2B = TCCR2B & ~(1<<2);

      TCCR2A = TCCR2A | (1<<5);
      TCCR2A = TCCR2A & ~(1<<4);

      if (reg=='A'){
        TCCR2A = TCCR2A | (1<<7);
        TCCR2A = TCCR2A & ~(1<<6);
      }

      else if (reg=='B'){
        TCCR2A = TCCR2A | (1<<5);
        TCCR2A = TCCR2A & ~(1<<4);}
   }
   
        
  if (pin==6 or pin==5){
      TCCR0B = TCCR0B & ~(1<<1);
      TCCR0A = TCCR0A | (11<<0);

      TCCR0B = TCCR0B | (1<<0);
      TCCR0B = TCCR0B & ~(1<<1);
      TCCR0B = TCCR0B & ~(1<<2);

      TCCR0A = TCCR0A | (1<<5);
      TCCR0A = TCCR0A & ~(1<<4);

      if (reg=='A'){
        TCCR0A = TCCR0A | (1<<7);
        TCCR0A = TCCR0A & ~(1<<6);
      }

      else if (reg=='B'){
        TCCR0A = TCCR0A | (1<<5);
        TCCR0A = TCCR0A & ~(1<<4);
      }
  }
  

  if (pin==9 or pin==10){
      TCCR1B = TCCR1B & ~(1<<1);
      TCCR1A = TCCR1A | (11<<0);

      TCCR1B = TCCR1B | (1<<0);
      TCCR1B = TCCR1B & ~(1<<1);
      TCCR1B = TCCR1B & ~(1<<2);

      TCCR1A = TCCR1A | (1<<5);
      TCCR1A = TCCR1A & ~(1<<4);

      if (reg=='A'){
        TCCR1A = TCCR1A | (1<<7);
        TCCR1A = TCCR1A & ~(1<<6);
      }

      else if (reg=='B'){
        TCCR1A = TCCR1A | (1<<5);
        TCCR1A = TCCR1A & ~(1<<4);
      }
      }
  }

void set_dutyCycle(int pin,int duty){
  if (pin==6) OCR0A = duty;
  else if (pin==5) OCR0B = duty;
  else if (pin==9) OCR1A = duty;
  else if (pin==10) OCR1B = duty;
  else if (pin==11) OCR2A = duty;
  else if (pin==3) OCR2B = duty;
}
