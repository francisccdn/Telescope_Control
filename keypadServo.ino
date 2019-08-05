#include <Keypad.h>
#include <Servo.h>

#define ALT_DOOR 11

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

Servo altitude;

void setup(){
  Serial.begin(9600);
  altitude.attach(ALT_DOOR);
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
    Serial.print("\n");
    altitude.write(numKey);
    ClearData();
  }
}

void ClearData(){
  while(inKeyIndex != 0){
    inKey[inKeyIndex--] = 0;
  }
  return;
}
