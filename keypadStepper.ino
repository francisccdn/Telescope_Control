#include <Stepper.h>
#include <Keypad.h>

#define E1 8
#define E2 9 
#define E3 10
#define E4 11

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[COLS] = {6, 7, 8, 9}; 
byte colPins[ROWS] = {2, 3, 4, 5}; 

char inKey[3];
byte inKeyIndex = 0;

byte numKey;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

const int passosPorGiro = 32;

byte currentAngle = 0; //Guarda o ultimo input de angulo para reajustar para o proximo de acordo

Stepper Mp(passosPorGiro, E1, E3, E2, E4);

void setup() {
    Serial.begin(9600);
}

void loop() {
  char readKey = customKeypad.getKey();
  
  if (readKey){
    inKey[inKeyIndex] = readKey; 
    inKeyIndex++; 
  }

  if (inKeyIndex == 3){   
    numKey = atol(inKey);
    
    Serial.print(numKey);
    Serial.print("\n");
    
    Motor(650, 1, (numKey - currentAngle)/360 ); 
    
    currentAngle = numKey;
    ClearData();
  }

}

void Motor(int vel, int sentido , float voltas){
  Mp.setSpeed(vel); // RPM
    
  for(int i = 1; i <= 64 * voltas; i++){
    Mp.step(passosPorGiro * sentido);
  }
  
  delay(5000);
}

void ClearData(){
  while(inKeyIndex != 0){
    inKey[inKeyIndex--] = 0; 
  }
  return;
}
