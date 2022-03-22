#ifndef _Output_H_
#define _Output_H_


typedef struct {
  bool modeValue;
  float tempValue;
  float positionValue;
} OutputValue;

typedef struct {
  char tempText[10];
  char positionText[10];
  char modeText[10];
} OutputVisual;

#endif