#include "gauge.h"
#include "input.h"
#include "output.h"
#include "average.h"
#include "motorController.h"


#define pin_motorControl1     12
#define pin_motorControl2     11
#define pin_motorControl3     10
#define pin_motorControl4     9

#define pin_pot               A6  // potentiometer wiper (middle terminal) connected to analog pin 3 outside leads to ground and +5V (0-1022)
#define pin_thermo            A2
#define pin_switch            2
#define pin_led               13

Gauge gauge(0, 1023);
Average avg;
Input input;
MotorController motor(pin_motorControl1, pin_motorControl2, pin_motorControl3, pin_motorControl4);

void setup() {  
  pinMode(pin_pot, INPUT); //potentiometer
  pinMode(pin_thermo, INPUT); //thermo  
  pinMode(pin_switch, INPUT_PULLUP); //switch
  pinMode(pin_led, OUTPUT); //led

  Serial.begin(9600);
  gauge.begin();  
  motor.begin();  
}

void loop() {

  unsigned short switchState, thermoRaw, manualRaw;
  OutputValue* input_ptr;

  switchState = digitalRead(pin_switch);
  digitalWrite(pin_led, switchState); 
  if (!switchState){
     manualRaw = avg.averageAnalogRead(pin_pot); Serial.print("m:");  Serial.println(manualRaw);
  }  
  thermoRaw = avg.averageAnalogRead(pin_thermo); Serial.print("t:");  Serial.println(thermoRaw);
 
  input_ptr = input.get(switchState, thermoRaw, manualRaw);

  gauge.updateScreen(input_ptr);

  motor.changePosition(input_ptr->positionValue);

  delay(30);
}