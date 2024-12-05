#include <HCSR04.h> // Biblioteca para o sensor ultrassônico

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
int dist = 0; // Este valor será passado pelo Qt

// Variável de controle do modo automático
bool autoParkingEnabled = false;

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
  analogWrite(velMotor1, vel1);
  analogWrite(velMotor2, vel2);

  Serial.println("Sistema inicializado. Pronto para receber comandos.");
}

void loop() {
  // Leitura de comandos via Serial
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove espaços em branco extras
    Serial.println("Comando recebido: " + command);

    if (command.startsWith("DIST:")) {
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
      stopMotors();
    }
    else {
      Serial.println("Comando desconhecido: " + command);
    }
  }

  // Modo de estacionamento automático
  if (autoParkingEnabled) {
    measureDist = distanceSensor.measureDistanceCm();
    Serial.print("Distância medida: ");
    Serial.print(measureDist);
    Serial.println(" cm");

    if (measureDist > dist + 2) {
      moveForward();
    } 
    else if (measureDist < dist - 2) {
      moveBack();
    } 
    else {
      stopMotors();
    }
  }
}

// Funções de controle dos motores
void moveForward() {
  analogWrite(velMotor1, vel2);
  analogWrite(velMotor2, vel2);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Movendo para frente");
}

void moveBack() {
  analogWrite(velMotor1, vel2);
  analogWrite(velMotor2, vel2);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("Movendo para trás");
}

void turnLeft() {
  analogWrite(velMotor1, vel2);
  analogWrite(velMotor2, vel2);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Virando à esquerda");
}

void turnRight() {
  analogWrite(velMotor1, vel2);
  analogWrite(velMotor2, vel2);
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