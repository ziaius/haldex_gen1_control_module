#include "output.h"

#ifndef _Input_H_
#define _Input_H_
      
class Input {
    public:
        Input();
        
        OutputValue* get(bool autoControlEnabled, unsigned short raw, unsigned short inputManualValue);
  
};


#endif