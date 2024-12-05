#include <HCSR04.h>
#include <SoftwareSerial.h>  // Biblioteca para comunicação Bluetooth

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
SoftwareSerial bluetooth(4, 7);  // RX, TX para o módulo HC-05

int vel1 = 200;  // Ajustado para um valor mais controlável
int vel2 = 150;

int measureDist = 0;
int dist = 0; // este valor será passado pelo Qt

// Variável de controle do modo automático
bool autoParkingEnabled = false;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

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

  analogWrite(velMotor1, vel1);
  analogWrite(velMotor2, vel2);
}

void loop() {
  // Leitura de comandos via Bluetooth
  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');
    command.trim(); // Remove espaços em branco extras

    if (command == "READ:DISTANCE") {
      int distance = distanceSensor.measureDistanceCm();
      bluetooth.print("DIST:");
      bluetooth.println(distance);
    }
    else if (command.startsWith("DIST:")) {
      dist = command.substring(5).toInt();
      Serial.print("Nova distância de parada: ");
      Serial.println(dist);
    }
    else if (command == "CMD:FORWARD") {
      moveForward();
    }
    else if (command == "CMD:BACK") {
      moveBack();
    }
    else if (command == "CMD:LEFT") {
      turnLeft();
    }
    else if (command == "CMD:RIGHT") {
      turnRight();
    }
    else if (command == "CMD:STOP") {
      stopMotors();
    }
    else if (command == "AUTO:ON") {
      autoParkingEnabled = true;
      Serial.println("Modo de estacionamento automático ativado");
    }
    else if (command == "AUTO:OFF") {
      autoParkingEnabled = false;
      Serial.println("Modo de estacionamento automático desativado");
    }
  }

  // Modo de estacionamento automático
  if (autoParkingEnabled) {
    measureDist = distanceSensor.measureDistanceCm();
    Serial.print(measureDist);
    Serial.println(" cm");

    if (measureDist > dist + 5) {
      moveForward();
    } 
    else if (measureDist < dist - 5) {
      moveBack();
    } 
    else {
      stopMotors();
    }
  }
}

// Funções de controle dos motores
void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Movendo para frente");
}

void moveBack() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("Movendo para trás");
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Virando à esquerda");
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("Virando à direita");
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("Parando os motores");
}