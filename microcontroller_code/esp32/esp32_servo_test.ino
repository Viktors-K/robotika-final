#include <ESP32Servo.h>
class webServo {
  private:
    int pin;
    int pos;
    int interval;
    Servo servo;
  public:
    webServo(int location, int timing = 10) {
      pin = location;
      interval = timing;
      pos = 0;
      pinMode(pin, OUTPUT);
      servo.write(pos);
    }

    void setup() {
      servo.setPeriodHertz(50);
      servo.attach(pin, 1000, 2000);
    }

    void moveTo(int newPos) {
      newPos = constrain(newPos, 0, 180);
      if (newPos > pos) {
        for (int i = pos; i <= newPos; i++) {
          pos = i;
          servo.write(pos);
          delay(interval);
        }
      } else if (newPos < pos) {
        for (int i = pos; i >= newPos; i--) {
          pos = i;
          servo.write(pos);
          delay(interval);
        }
      }
    }
    void sweepServo() {
      moveTo(0);
      moveTo(180);
      moveTo(0);
    }
};

webServo servo1(14);

void setup() {
  Serial.begin(115200);
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	servo1.setup();
}

void loop() {
  Serial.println("sweep");
  servo1.sweepServo();
  Serial.println("stop sweep");
  delay(2000);
  Serial.println("move to 100");
  servo1.moveTo(100);
  Serial.println("stop move to 100");
  delay(2000);
  Serial.println("move to 0");
  servo1.moveTo(0);
  Serial.println("stop move to 0");
  delay(2000);
  Serial.println("move to 180");
  servo1.moveTo(180);
  Serial.println("stop move to 180");
  delay(2000);
}



