#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Keypad.h>
#include <Servo.h>

#define ALT_DOOR 5
#define E1 53
#define E2 51
#define E3 49
#define E4 47

#define ROWS 4
#define COLS 4

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//                    r1  r2  r3  r4
byte rowPins[COLS] = {38, 36, 34, 32};
//                    c1  c2  c3  c4
byte colPins[ROWS] = {28, 26, 24, 22};

byte customChar[] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

char inKey[3];
byte inKeyIndex = 0;
int numKey;

int currentAngle = 0; //Guarda o ultimo input de angulo para reajustar para o proximo de acordo
const int passosPorGiro = 32;
float coefAngular;

byte dadoRecebido = 0;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Stepper azimuth(passosPorGiro, E1, E3, E2, E4);
Servo altitude;

void setup() {
    altitude.attach(ALT_DOOR);
    lcd.init();
    lcd.createChar(0, customChar);
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
  if (dadoRecebido == 0 && inKeyIndex == 3){
    numKey = atol(inKey);

    coefAngular = (numKey - currentAngle)/360.0;

    lcd.write(0);
    lcd.setCursor(11, 1);

    if(coefAngular < 0){ //Permite que o motor gire nas duas direções
        Motor(1, coefAngular);
    }else{
        Motor(-1, coefAngular);
    }

    currentAngle = numKey;
    ClearData();

    dadoRecebido = 1; //Indica que o motor recebeu o dado
  }


  //Manda o input para o servo
  if (dadoRecebido == 1 && inKeyIndex == 3){
    numKey = atol(inKey);

    lcd.write(0);
    lcd.noCursor();

    altitude.write(numKey);
    ClearData();

    dadoRecebido = 2; //Indica que o servo recebeu o dado
  }


  //Reseta o LCD quando uma nova tecla é pressionada
  //(sem isso, o LCD resetaria automaticamente)
  if (dadoRecebido == 2 && inKeyIndex == 1){ //se os dados já foram enviados para o motor e o servo, e uma tecla foi pressionada
    LcdReset();
    dadoRecebido = 0; //Indica que o usuario deseja fazer um novo input
    ClearData(); //Limpa a tecla que foi digitada para resetar
  }
}

void Motor(int sentido , float voltas){
  if(voltas < 0){
    voltas = voltas * -1;
  }

  azimuth.setSpeed(650); // RPM

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
    inKey[--inKeyIndex] = 0;
  }
}
