#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "ADC.h"  //may need to replace with irrigation adc library to edit the initialization sequence to close and clear first
#include "dio.h"
#include "uart.h"

void init();

int upper_led = 3;
int lower_led = 2;
int sensor_pin = 1;
int val = 0;

int upper_thresh = 55;
char upper_thresh_str[2];
int lower_thresh = 50;
char lower_thresh_str[2];

int prev_temp = 0;
int temp_val = 0;
char temp_str[4];

int main(void) {

  init();  // Call Initialization Function

  while (1) {
    //check if the buttons are adjusted, adjust the thresholds accordingly
    val = ADC_Read(0);
    if (val==0) lower_thresh += 1; 
    else if (val<131) upper_thresh -= 1;
    else if (val<305) upper_thresh+=1;
    else if (val<479) upper_thresh -= 1;
    else if (val<722) lower_thresh -= 1;

    itoa(upper_thresh, upper_thresh_str, 10);
    itoa(lower_thresh, lower_thresh_str, 10);
Uart_SendString("\n Upper:");    
Uart_SendString(upper_thresh_str);

Uart_SendString("\n Lower:");
Uart_SendString(lower_thresh_str);


    temp_val = ADC_Read(sensor_pin);

    //change the lcd display if different
    if (temp_val != prev_temp) {
      itoa(temp_val, temp_str, 10);
      LCD_String_xy(0, 0, "Temperature: ");
      LCD_String_xy(0, 12, temp_str);
      //LCD_String_xy(0, 14, "°C");

      //take action on leds
      if (temp_val > upper_thresh) {
        Dio_SetPinState('D', lower_led, 0);
        Dio_SetPinState('D', upper_led, 1);
        LCD_String_xy(1, 0, "High: Cooling ON");
      } else if (temp_val < lower_thresh) {
        Dio_SetPinState('D', upper_led, 0);
        Dio_SetPinState('D', lower_led, 1);
        LCD_String_xy(1, 0, "Low: Heating ON");
      } else {
        Dio_SetPinState('D', lower_led, 0);
        Dio_SetPinState('D', upper_led, 0);
        LCD_String_xy(1, 0, "Normal          ");
      }

      prev_temp = temp_val;
      _delay_ms(500);
    }
    
  }
  return 0;
}

void init() {
  Uart_Init();
  ADC_Init();
  LCD_Init();
  Dio_SetPinDirection('D', sensor_pin, 'i');
  Dio_SetPinDirection('D', upper_led, 'o');
  Dio_SetPinDirection('D', lower_led, 'o');
}