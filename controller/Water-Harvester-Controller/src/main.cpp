#include <Arduino.h>


void setup() {

  Serial.begin(9600);
  
}

void loop() {

  int i = 0;
  i++;
  Serial.println(i);  
}

