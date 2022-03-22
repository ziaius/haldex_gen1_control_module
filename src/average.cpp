#include <Arduino.h> 
#include "average.h"


Average::Average() {}

unsigned short Average::averageAnalogRead(int pin)
{
  unsigned short buffer = 0;
  const char numb = 5;

  for (char i = 0; i < numb; i++)
  {
    buffer += analogRead(pin);   
    delay(1);
  }
  return buffer / numb;
} 
