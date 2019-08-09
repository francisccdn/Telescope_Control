#include <Stepper.h>
#include <Keypad.h>

#define E1 47
#define E2 49
#define E3 51
#define E4 53

const byte ROWS = 4; 
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//                    r1  r2  r3  r4
byte rowPins[COLS] = {32, 34, 36, 38};
//                    c1  c2  c3  c4
byte colPins[ROWS] = {28, 26, 24, 22};

char inKey[3];
byte inKeyIndex = 0;

byte numKey;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

const int passosPorGiro = 32;

int currentAngle = 0; //Guarda o ultimo input de angulo para reajustar para o proximo de acordo
float coefAngular;

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

    coefAngular = (numKey - currentAngle)/360.0;

    Serial.print(numKey);
    Serial.print(" DIGITADO \n");
    Serial.print(coefAngular);
    Serial.print(" PARAMETRO \n");

    if(coefAngular < 0){
        Motor(650, -1, coefAngular * -1);
    }else{
        Motor(650, 1, coefAngular);
    }

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
