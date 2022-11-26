#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "ADC.h"                          //may need to replace with irrigation adc library to edit the initialization sequence to close and clear first
#include "dio.h"
#include <dht.h>

// Function Prototypes
void init();

int temp_led = 2;
int humidity_led = 3;

int temp_thresh = 25;
int humidity_thresh = 100;

int prev_temp = 0;
int prev_lcd = 0;
int temp_val = 0;
int humidity_val = 0;

int sensor_ch = 1;

int main(void){

  init();     // Call Initialization Function

  while (1)
  {
    
   //get the temprature and humidity value (possibly via timer)
   //change the lcd display if different
   //check if the buttons are adjusted, adjust the thresholds accordingly
   //take action on leds
  }
  return 0; 
}

void init(){
  ADC_Init();
  LCD_Init();
}