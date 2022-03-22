#ifndef _MotorController_H_
#define _MotorController_H_

#include <Stepper.h>
      
class MotorController {
    public:
        MotorController(int pin1, int pin2, int pin3, int pin4);
        void begin();
        short changePosition(short inputValue);  
    private: 
        Stepper motor; 
};


#endif