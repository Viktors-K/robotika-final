#define servoBottom_readPin 2
#define servoBottom_writePin 8
#define servoLeft_readPin 3
#define servoLeft_writePin 7
#define servoRight_readPin 4
#define servoRight_writePin 9
#define servoHand_readPin 5
#define servoHand_writePin 6
#include <Servo.h>

Servo servoBottom;
Servo servoLeft;
Servo servoRight;
Servo servoHand;

int servoBottom_pos = 0;
int servoLeft_pos = 0;
int servoRight_pos = 0;
int servoHand_pos = 0;

void setup() {
  pinMode(servoBottom_readPin, INPUT);
  pinMode(servoLeft_readPin, INPUT);
  pinMode(servoRight_readPin, INPUT);
  pinMode(servoHand_readPin, INPUT);
  servoBottom.attach(servoBottom_writePin);
  servoLeft.attach(servoLeft_writePin);
  servoRight.attach(servoRight_writePin);
  servoHand.attach(servoHand_writePin);
  Serial.begin(115200);
  Serial.println("App started.");
}

void loop() {
  int servoBottom_pinState = digitalRead(servoBottom_readPin);
  int servoLeft_pinState = digitalRead(servoLeft_readPin);
  int servoRight_pinState = digitalRead(servoRight_readPin);
  int servoHand_pinState = digitalRead(servoHand_readPin);
  Serial.println("Bottom: " + servoBottom_pinState);
  Serial.println("Left: " + servoLeft_pinState);
  Serial.println("Right: " + servoRight_pinState);
  Serial.println("Hand: " + servoHand_pinState);
  if (servoBottom_pinState == 1) {
    sweepServo(servoBottom);
  }
  if (servoLeft_pinState == 1) {
    sweepServo(servoLeft);
  }
  if (servoRight_pinState == 1) {
    sweepServo(servoRight);
  }
  if (servoHand_pinState == 1) {
    sweepServo(servoHand);
  }
  delay(1000);
}

void sweepServo(Servo &servo) {
  for (int pos = 0; pos <= 180; pos++) {
    servo.write(pos);
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos--) {
    servo.write(pos);
    delay(15);
  }
}
