#include <Keypad.h>
#include <Servo.h>

#define AZI_DOOR 11

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {6, 7, 8, 9}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

char inKey[3];
byte inKeyIndex = 0;

byte numKey;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

Servo azimuth;

void setup(){
  Serial.begin(9600);
  azimuth.attach(AZI_DOOR);
}
  
void loop(){
  char readKey = customKeypad.getKey();
  
  if (readKey){
    inKey[inKeyIndex] = readKey; 
    inKeyIndex++; 
  }

  if (inKeyIndex == 3){
    numKey = atol(inKey);
    Serial.print(numKey);
    azimuth.write(numKey);
    ClearData();
  }
}

void ClearData(){
  while(inKeyIndex != 0){
    inKey[inKeyIndex--] = 0; 
  }
  return;
}
