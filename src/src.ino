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


MotorController motor(pin_motorControl1, pin_motorControl2, pin_motorControl3, pin_motorControl4);
Gauge gauge(0, 1023);
Input input;
Average avg;

unsigned short isECUcontrolOn, thermoRawValue, manualControlRawValue;
OutputValue* input_ptr;

void setup() {  
  pinMode(pin_pot, INPUT);
  pinMode(pin_thermo, INPUT);
  pinMode(pin_switch, INPUT_PULLUP);
  pinMode(pin_led, OUTPUT);

  Serial.begin(9600);
  motor.begin();
  gauge.begin();
}

void loop() {

  //read button: ECU control / potentiomenter control
  isECUcontrolOn = digitalRead(pin_switch);
  //led ON ECU controll/ led OFF potetiomenter control
  digitalWrite(pin_led, isECUcontrolOn);
  
  //read potentiomener raw value for manual control
  if (!isECUcontrolOn){
     manualControlRawValue = avg.averageAnalogRead(pin_pot); Serial.print("m:");  Serial.println(manualControlRawValue);
  }  
  
  //read temperature resistor value
  thermoRawValue = avg.averageAnalogRead(pin_thermo); Serial.print("t:");  Serial.println(thermoRawValue);
 
  int potV = map(analogRead(A0),0,1024,0,360);

  //parse raw input values 
  input_ptr = input.get(isECUcontrolOn, thermoRawValue, manualControlRawValue);

  gauge.updateScreen(input_ptr);

  motor.changePosition(input_ptr->positionValue);

  delay(30);
}