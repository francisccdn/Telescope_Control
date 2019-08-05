#include <Stepper.h>
#define E1 8
#define E2 9 
#define E3 10
#define E4 11

const int passosPorGiro = 32;

Stepper Mp(passosPorGiro,E1,E3,E2,E4);

void setup() {

 }

void loop() {
motor(650,1,5);
//motor(650,-1,1);
//motor(800,1,1);
//motor(650,1,3);

while(1){

    }
}

void motor(int vel, int sentido , float voltas){
    Mp.setSpeed(vel); // RPM
   for(int i = 1;i<=64*voltas;i++){
    
   Mp.step(passosPorGiro*sentido);
  }
  
  delay(5000);
 }
