#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"
#include "LCD.h"

#define setBit(sfr, bit)     (_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)   (_SFR_BYTE(sfr) &= ~(1 << bit))
#define getBit(sfr, bit)     ((_SFR_BYTE(sfr)>>bit)&(1<<bit)!=0)
#define toggleBit(sfr, bit)  (_SFR_BYTE(sfr) ^= (1 << bit))

#define Dio_SetPinDirection(port, bit, direction) ((direction==1) ? (setBit(port, bit)) : (clearBit(port, bit)))
#define Dio_SetPinState(port, bit, state) ((state==1) ? (setBit(port, bit)) : (clearBit(port, bit)))
#define Dio_GetPinState(port, bit)  (getBit(port, bit))

#define LOWER_LED PD2
#define LOWER_LED_DDR DDRD
#define LOWER_LED_PORT PORTD

#define UPPER_LED PD3
#define UPPER_LED_DDR DDRD
#define UPPER_LED_PORT PORTD

#define LM_CH 1
#define LM_DDR DDRC

#define BUTTON_CH 0
#define BUTTON_DDR DDRC

void init();

/*Global upper and lower temperature thresholds values*/
int lower= 50;
int upper = 55;
char _str[2];


int prev_temp = 0;  //previous tempertemperature value
int temp_val = 0;   //current temperature value
char temp_str[4];

int button = 0;

int main(void) {

  init();  // Call Initialization Function

  while (1) {
    //check if the buttons are adjusted, adjust the thresholds accordingly
    button = ADC_Read(0);
    if (button < 131) lower += 1;
    else if (button < 305) upper += 1;
    else if (button < 479) upper -= 1;
    else if (button < 722) lower -= 1;

    itoa(upper, _str, 10);
    LCD_String_xy(1, 5, _str);
    itoa(lower, _str, 10);
    LCD_String_xy(1, 0, _str);
    

    temp_val = ADC_Read(LM_CH);

    //change the lcd display if different
    if (temp_val != prev_temp) {
      itoa(temp_val, temp_str, 10);
      LCD_String_xy(0, 0, "Temp: ");
      LCD_String_xy(0, 5, temp_str);
      LCD_String_xy(0, 9, "C");

      //take action on leds
      if (temp_val > upper) {
        Dio_SetPinState(LOWER_LED_PORT, LOWER_LED, 0);
        Dio_SetPinState(UPPER_LED_PORT, UPPER_LED, 1);
        LCD_String_xy(0, 10, "  High");
      } else if (temp_val < lower) {
        Dio_SetPinState(UPPER_LED_PORT, UPPER_LED, 0);
        Dio_SetPinState(LOWER_LED_PORT, LOWER_LED, 1);
        LCD_String_xy(0, 10, "   Low");
      } else {
        Dio_SetPinState(UPPER_LED_PORT, UPPER_LED, 0);
        Dio_SetPinState(LOWER_LED_PORT, LOWER_LED, 0);
        LCD_String_xy(0, 10, "Normal");
      }

      prev_temp = temp_val;
      _delay_ms(500);
    }
  }
  return 0;
}

void init() {
  ADC_Init();
  LCD_Init();

  Dio_SetPinDirection(LM_DDR, LM_CH, 0);
  Dio_SetPinDirection(BUTTON_DDR, BUTTON_CH, 0);

  Dio_SetPinDirection(LOWER_LED_DDR, LOWER_LED, 1);
  Dio_SetPinDirection(UPPER_LED_DDR, UPPER_LED, 1);
}