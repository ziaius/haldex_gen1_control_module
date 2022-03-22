#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "gauge.h"


#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64);

#define pi 3.14159265359
#define tau 2 * pi

float startAngleD, startAngle;
float endAngleD, endAngle;
int centerX, centerY, radius;
int gaugeMin, gaugeMax;

  
OutputVisual out1;  
OutputVisual *out1_ptr = &out1;

Gauge::Gauge(int min, int max) {
    float centerD = 270; //Angle where the center of the gauge will be
    float widthD = 40; //Angle that the gauge will be wide
    
    startAngleD = centerD - widthD;
    endAngleD   = centerD + widthD;
  
    centerX    = 63;//    Center of arc X (pixels)
    centerY    = 110;//    Center of arc Y (pixels)
    radius     = 65;//    Radious of arc (pixels)
  
    startAngle = startAngleD / 360 * tau;
    endAngle   = endAngleD   / 360 * tau;

    gaugeMin = min;
    gaugeMax = max;
}

char* floatToString(float value) {
    char afr[6];
    dtostrf( value, 5, 2, afr);       // float, width, precision, buffer
    return afr;
}
  
float scale(float inScaleMin, float inScaleMax, float outScaleMin, float outScaleMax, float value){
    return ((value - inScaleMin) / (inScaleMax - inScaleMin) * (outScaleMax-outScaleMin)) + outScaleMin;
}  
float angleToXD(float centerX, float radius, float angleD) {
    float angle = (angleD / 360) * (tau);
    return centerX+radius*cos(angle); // Calculate arc point (X)
}
float angleToYD(float centerY, float radius, float angleD) {
    float angle = (angleD / 360) * (tau);
    return centerY+radius*sin(angle); // Calculate arc point (Y)
}
void drawArc(float startAngle, float endAngle, float segments, int centerX, int centerY, int radius) {
    float resolution = (endAngle-startAngle)/segments; // Calculates steps in arc based on segments
    float x = centerX+radius*cos(startAngle); // Calculate start point of arc (X)
    float y = centerY+radius*sin(startAngle); // Calculate start point of arc (Y)
    display.writePixel(x,y,WHITE); // Place starting point of arc
  
    for (float angle = startAngle; angle < endAngle; angle += resolution) { // Sweep arc
        x = centerX+radius*cos(angle); // Calculate arc point (X)
        y = centerY+radius*sin(angle); // Calculate arc point (Y)
        display.writePixel(x,y,WHITE);
    }
}
void drawNeedle(float angle, float startAngle, float endAngle, float centerX, float centerY, int radius, int color){
    float leftX = angleToXD(centerX, radius+5, angle - 5);
    float leftY = angleToYD(centerY, radius+5, angle - 5);
  
    float rightX = angleToXD(centerX, radius+5, angle + 5);
    float rightY = angleToYD(centerY, radius+5, angle + 5);
  
    float topX = angleToXD(centerX, radius+20, angle);
    float topY = angleToYD(centerY, radius+20, angle);
    //drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    //display.drawLine(topX, topY-20, topX, topY, color);
    display.fillTriangle(leftX,leftY,topX,topY,rightX,rightY,color);
}
void drawGaugeLines(float startAngle, float endAngle, float centerX, float centerY, int radius){
    drawArc(startAngle, endAngle, 150, centerX, centerY, radius + 23);
    drawArc(startAngle, endAngle, 110, centerX, centerY, radius + 1);
    drawArc(startAngle, endAngle, 110, centerX, centerY, radius + 4);
}

void drawGaugeFrame() {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    
    // display.setCursor(0, 0);
    // display.println("Rich");
    // display.setCursor(100, 0);
    // display.println("Lean");
  
    // display.setTextSize(2);
    // display.setCursor(45,15);
    // display.println("AFR");
  
    drawGaugeLines(startAngle, endAngle, centerX, centerY, 65);
}

void drawGaugeData(float value) {
    float angle = scale(gaugeMin,gaugeMax,startAngleD,endAngleD,value); 
    //char* afr = floatToString(value);  
    
    drawNeedle(angle, startAngle, endAngle, centerX, centerY, radius, INVERSE);
   
}

void drawThermometer(float value){
  short level = 0;

  if (value > 14500){
    level = 4;
  }
  else if (value > 5000){
    level = 3;
  }
  else if (value > 4000){
    level = 2;
  }
  else if (value > 3000){
    level = 1;
  }
  else if (value > 2000){
    level = 0;
  }
  display.drawBitmap(114, 5, temperature1_5[level] , 11, 29,  INVERSE, BLACK);
}

void printtext(String line1, String line2, String line3, String line4) {  
  display.setTextColor(INVERSE);
  display.setTextSize(2);
  display.setCursor(0, 0);  
  display.println(line1);

  
  display.setTextSize(2);
  display.setCursor(29, 50);  
  display.print(line2); 
  display.println("%");   


  display.setTextSize(1);
  display.setCursor(85, 0); 
  display.println(line3);
  //display.println(line4);
}


void Gauge::updateScreen(OutputValue* input_ptr) {
  if ((*input_ptr).modeValue == true){
    sprintf((*out1_ptr).modeText, "auto");
  }
  else {
    sprintf((*out1_ptr).modeText, "manual");
  } 
  char str_temp[5];
  dtostrf((input_ptr->positionValue/gaugeMax*100), 4, 0, str_temp);
  sprintf((*out1_ptr).positionText, "%s", str_temp);  
  dtostrf(input_ptr->tempValue, 5, 0, str_temp);
  sprintf((*out1_ptr).tempText, "%s", str_temp);
  
  printtext(out1_ptr->modeText, out1_ptr->positionText, out1_ptr->tempText, "");
  drawGaugeData(input_ptr->positionValue);
  drawThermometer(input_ptr->tempValue);
  display.display();  
  drawThermometer(input_ptr->tempValue);
  drawGaugeData(input_ptr->positionValue);
  printtext(out1_ptr->modeText, out1_ptr->positionText, out1_ptr->tempText, "");
}

void Gauge::begin() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);     
    drawGaugeFrame();
}

