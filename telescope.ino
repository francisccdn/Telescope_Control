#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Keypad.h>
#include <Servo.h>

#define ALT_DOOR 5
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

byte numKey, indicador = 0;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal_I2C lcd(0x27,20,4);

const int passosPorGiro = 32;

byte currentAngle = 0; //Guarda o ultimo input de angulo para reajustar para o proximo de acordo

Stepper azimuth(passosPorGiro, E1, E3, E2, E4);

Servo altitude;

void setup() {
    Serial.begin(9600);
    altitude.attach(ALT_DOOR);
    lcd.init();
    lcd.backlight();
    lcd.cursor();
    LcdReset();
}

void loop() {
  char readKey = customKeypad.getKey();

  if (readKey){
    inKey[inKeyIndex] = readKey; 
    lcd.print(inKey[inKeyIndex]);
    inKeyIndex++; 
  }

//Manda o input para o motor

  if (inKeyIndex == 3 && indicador == 0){   
    numKey = atol(inKey);

    lcd.print("º");
    lcd.setCursor(11, 1);
    
    Serial.print(numKey);
    Serial.print("º\n");
    
    Motor(650, 1, (numKey - currentAngle)/360 ); 
    
    currentAngle = numKey;
    ClearData();

    indicador = 1; //Indica que o motor recebeu o dado
  }

//Manda o input para o servo

  if (inKeyIndex == 3 && indicador == 1){
    numKey = atol(inKey);
    
    lcd.print("º");
    Serial.print(numKey);
    Serial.print("\n");
    
    altitude.write(numKey);
    ClearData();

    indicador = 2; //Indica que o servo recebeu o dado
  }

/*Reseta o LCD quando uma nova tecla é pressionada
(sem isso, o LCD resetaria automaticamente)*/

  if (indicador == 2 && inKeyIndex == 1){ //se os dados já foram enviados para o motor e o servo, e uma tecla foi pressionada
    LcdReset();
    indicador = 0;
  }

}

void Motor(int vel, int sentido , float voltas){
  azimuth.setSpeed(vel); // RPM
    
  for(int i = 1; i <= 64 * voltas; i++){
    azimuth.step(passosPorGiro * sentido);
  }
  
  delay(5000);
}

void LcdReset(){
  lcd.clear();
  lcd.print("Azimuth  : ");
  lcd.setCursor(0, 1);
  lcd.print("Altitude : ");
  lcd.setCursor(11, 0);
}

void ClearData(){
  while(inKeyIndex != 0){
    inKey[inKeyIndex--] = 0; 
  }
  return;
}
