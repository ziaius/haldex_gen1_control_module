#include <stdio.h>
#include "input.h"

//static short my_array[20] = {1023/1, 1023/2, 1023/4, 1023/8, 1023/16, 1023/32, 1023/64, 1023/128, 1023/256, 1023/512, 1023/1023, 1023/512, 1023/256, 1023/128, 1023/64, 1023/1023, 1023/1, 1023/1023, 1023/1, 1023/1023};
static short my_array[20] = {0, 512, 1023, 512, 256, 512, 1023, 0, 32,64, 128, 256, 512, 1023, 512, 256, 125, 64, 32, 16};
unsigned int jj = 0;

OutputValue out;  
OutputValue *out_ptr = &out;

float getTemperature(unsigned short Vout_raw, OutputValue* out_ptr)
{
  const float Vin = 5.2;//4.95;
  const float R1 = 2200;
  float Vout = 0;
  float R2 = 0;
  float buffer = 0;

  if(Vout_raw)
  {    
    buffer = Vout_raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) - 1;
    R2 = R1 * buffer;
  }

  (*out_ptr).tempValue = R2;
}

void getPositionInput(bool autoControlEnabled, unsigned short inputManualValue, OutputValue* out_ptr){
  
  if (!autoControlEnabled) 
  {
    out_ptr->modeValue = false;
    (*out_ptr).positionValue = inputManualValue;
  }
  else if (autoControlEnabled)
  { 
    
    out_ptr->modeValue = true;
    (*out_ptr).positionValue = my_array[jj%20];
    jj=jj+1;
  }
}

Input::Input() {}

OutputValue* Input::get(bool autoControlEnabled, unsigned short Vout_raw, unsigned short inputManualValue)
{    
  getTemperature(Vout_raw, out_ptr);
  getPositionInput(autoControlEnabled, inputManualValue, out_ptr);

  return out_ptr;
}