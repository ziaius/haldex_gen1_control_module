#include <Stepper.h>
#include <math.h>
#include "motorController.h"

char _rpm = 70;//30;//200;// 600 5V OK
//double _degreesPerStep = 1.8;
short _maxPosition = 83; // 150/_degreesPerStep; // 83.33 steps
short _stepsPerRevolution = 200; // 360/_degreesPerStep;

short currentPossition;

 //(200, pin_motorControl1, pin_motorControl2, pin_motorControl3, pin_motorControl4); 
MotorController::MotorController(int pin1, int pin2, int pin3, int pin4) : motor(_stepsPerRevolution, pin1, pin2, pin3, pin4)
{
  currentPossition = 0;
}

void MotorController::begin()
{
  motor.setSpeed(_rpm);
}

short mapInputToRequiredPosition(short input) //0-1023 //5V in 10bits
{
  return input / 1023 * _maxPosition;
}

short getDeltaPosition(short inputValue, short currentPossition)
{ 
  short nextPosition = mapInputToRequiredPosition(inputValue);
  /*
    20-0=+20
    0-20=-20
    30-0=+30
    36-30=+6  */
  return nextPosition - currentPossition;
}

short MotorController::changePosition(short inputValue)
{
 short deltaPosition = getDeltaPosition(inputValue,  currentPossition);
 
  if (deltaPosition >= 0)
  {
    motor.step(deltaPosition);
  }
  else
  {
    motor.step(deltaPosition);
  }
  currentPossition += deltaPosition;

  return currentPossition;
}