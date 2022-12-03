#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"
#include "LCD.h"
#include "pwm.h"

#define _BV(bit)  (1<<bit)
#define BV(bit)  (1<<bit)
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))
#define loop_until_bit_is_set(sfr, bit) do { } while (bit_is_clear(sfr, bit))
#define loop_until_bit_is_clear(sfr, bit) do { } while (bit_is_set(sfr, bit))

#define setBit(sfr, bit)     (_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)   (_SFR_BYTE(sfr) &= ~(1 << bit))
#define toggleBit(sfr, bit)  (_SFR_BYTE(sfr) ^= (1 << bit))

#define Dio_SetPinDirection(sfr, bit, direction) ((direction==1) ? (setBit(sfr, bit)) : (clearBit(sfr, bit)))
#define Dio_SetPinState(sfr, bit, state) ((state==1) ? (setBit(sfr, bit)) : (clearBit(sfr, bit)))
#define Dio_GetPinState(sfr, bit)  (bit_is_set(sfr, bit))

#define LOWER_LED PB3
#define LOWER_LED_DDR DDRB
#define LOWER_LED_PORT PORTB

#define UPPER_LED PD3
#define UPPER_LED_DDR DDRD
#define UPPER_LED_PORT PORTD

#define LM_CH 1
#define LM_DDR DDRC

#define BUTTON_CH 0
#define BUTTON_DDR DDRC

void init();

/*Global upper and lower temperature thresholds values*/
int lower = 25;
int upper = 40;

/*Previous and current temperature readings from LM35*/
int prev_temp = 0;
int temp_val = 0;

int button = 0;  //ADC CH0 reading connected to multiple buttons on the LCD Shield
char _str[3];    //global string buffer for converting from numerical to str and displaying on the LCD

int main(void) {

  init();  // Call Initialization Function

  while (1) {

    //check if the buttons are adjusted, adjust the thresholds accordingly
    button = ADC_Read(0);
    if ((button < 131)) {
      if ((lower + 1) > upper) continue;  //in case the lower limit would be higher than the upper limit
      lower += 1;
      _delay_ms(120);  //delays added for debouncing
    } else if (button < 305) {
      upper += 1;
      _delay_ms(120);
    } else if ((button < 479)) {
      if ((upper - 1) < lower) continue;
      upper -= 1;
      _delay_ms(120);
    } else if (button < 722) {
      lower -= 1;
      _delay_ms(120);
    }

    //print the upper and lower thresholds on the LCD
    itoa(upper, _str, 10);
    LCD_String_xy(1, 11, _str);
    itoa(lower, _str, 10);
    LCD_String_xy(1, 5, _str);

    //Timer1 (Counter): Checking the temperature only after certain amounts of time
    if (TCNT1 > 64000) {
      temp_val = ADC_Read(LM_CH);
      /*converting from voltage to Celsius*/
      temp_val /= 4;
      temp_val += 20;
      /*resetting the clock*/
      TCNT1 = 0;
    }

    //change the lcd display if different
    if (temp_val != prev_temp) {
      itoa(temp_val, _str, 10);
      LCD_String_xy(0, 5, _str);

      //take action on leds if the temperature is outside the lower to upper temperature threshold range
      if (temp_val > upper) {
        set_dutyCycle(11, 0);
        /*PWM: light the LED proportionally to the difference between the temperature and the upper threshold*/
        set_dutyCycle(3, (temp_val - upper) * 10);
        LCD_String_xy(0, 10, "  High");
      } else if (temp_val < lower) {
        set_dutyCycle(3, 0);
        /*PWM: light the LED proportionally to the absolute difference between the temperature and the lower threshold*/
        set_dutyCycle(11, (lower - temp_val) * 10);
        LCD_String_xy(0, 10, "   Low");
      } else {
        set_dutyCycle(11, 0);
        set_dutyCycle(3, 0);
        LCD_String_xy(0, 10, "Normal");
      }

      prev_temp = temp_val;
      _delay_ms(10);
    }
  }
  return 0;
}

void init() {
  LCD_Init();
  LCD_String_xy(0, 0, "Temp: ");
  LCD_String_xy(0, 7, " C");
  LCD_String_xy(1, 0, "From ");
  LCD_String_xy(1, 7, " To ");
  LCD_String_xy(1, 14, "C");

  ADC_Init();
  //initializing pin A0 (ADC Channel 0) as input
  Dio_SetPinDirection(BUTTON_DDR, BUTTON_CH, 0);

  //setting upper and lower threshold leds pins as output
  Dio_SetPinDirection(LOWER_LED_DDR, LOWER_LED, 1);
  Dio_SetPinDirection(UPPER_LED_DDR, UPPER_LED, 1);
  //initializing led pins to be pwm (using timer 2)
  pwm_init(11, 'A');
  pwm_init(3, 'B');

  //initializing timer 1 as a counter
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TCNT1 = 0;
}