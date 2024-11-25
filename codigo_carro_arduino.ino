#include <HCSR04.h>

#define velMotor1 2
#define velMotor2 3
#define in1 8
#define in2 9
#define in3 10
#define in4 11
#define tmp 10
#define pEcho 5
#define pTrigger 6

UltraSonicDistanceSensor distanceSensor(pTrigger, pEcho);

int vel1 = 255;

int vel2 = 129;

int measureDist = 0;

int dist = 15; // este valor sera passado pelo qt


void setup() {
  Serial.begin(9600);

  pinMode(velMotor1, OUTPUT);
  pinMode(velMotor2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(velMotor1,vel1);
  analogWrite(velMotor2,vel1);
  analogWrite(velMotor1,vel2);
  analogWrite(velMotor2,vel2);
}

void loop() {
  measureDist = distanceSensor.measureDistanceCm();
  Serial.print(measureDist);
  Serial.println("cm");

  analogWrite(velMotor1,vel2);
  analogWrite(velMotor2,vel2);

  if(dist < measureDist - 5){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else if(dist > measureDist + 5){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  analogWrite(velMotor1,vel1);
  analogWrite(velMotor2,vel1);

  if(dist < measureDist){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else if(dist > measureDist){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  else if(dist == measureDist){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }
  else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  


  
}





















