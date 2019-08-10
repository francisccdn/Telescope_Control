#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Keypad.h>
#include <Servo.h>

#define ALT_DOOR 5
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

byte numKey, indicador = 0;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27,20,4);

const int passosPorGiro = 32;

byte currentAngle = 0; //Guarda o ultimo input de angulo para reajustar para o proximo de acordo

float coefAngular;

Stepper azimuth(passosPorGiro, E1, E3, E2, E4);

Servo altitude;

void setup() {
    Serial.begin(9600);
    altitude.attach(ALT_DOOR);
    lcd.init();
    lcd.backlight();
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

  if (indicador == 0 && inKeyIndex == 3){
    numKey = atol(inKey);

    coefAngular = (numKey - currentAngle)/360.0;

    lcd.print("º");
    lcd.setCursor(11, 1);

    Serial.print(numKey);
    Serial.print("º\n");
    Serial.print(" DIGITADO \n");
    Serial.print(coefAngular);
    Serial.print(" PARAMETRO \n");

    if(coefAngular < 0){ //Permite que o motor gire nas duas direções
        Motor(650, -1, coefAngular * -1);
    }else{
        Motor(650, 1, coefAngular);
    }
    
    currentAngle = numKey;
    ClearData();

    indicador = 1; //Indica que o motor recebeu o dado
  }


//Manda o input para o servo

  if (indicador == 1 && inKeyIndex == 3){
    numKey = atol(inKey);

    lcd.print("º");
    lcd.noCursor();
    
    Serial.print(numKey);
    Serial.print(" SERVO \n");

    altitude.write(numKey);
    ClearData();

    indicador = 2; //Indica que o servo recebeu o dado
  }


/*Reseta o LCD quando uma nova tecla é pressionada
(sem isso, o LCD resetaria automaticamente)*/

  if (indicador == 2 && inKeyIndex == 1){ //se os dados já foram enviados para o motor e o servo, e uma tecla foi pressionada
    LcdReset();
    indicador = 0; //Indica que o usuario deseja fazer um novo input
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
  lcd.cursor();
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
