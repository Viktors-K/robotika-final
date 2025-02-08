#include <WiFi.h>
#include <ESP32Servo.h>
class webServo {
  private:
    int pin;
    int pos;
    int interval;
    Servo servo;
  public:
    int getPosition() {
      return pos;
    }
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
	void moveBy(int increment) {
		int newPos = pos + increment;
		moveTo(newPos); 
	}
    void sweepServo() {
      moveTo(0);
      moveTo(180);
      moveTo(0);
    }
};
#define servoBottom_writePin 26
#define servoLeft_writePin 12
#define servoRight_writePin 14
#define servoHand_writePin 27

const char* ssid = "Robot_Arm";
const char* password = "Biete302";

webServo servoBottom(servoBottom_writePin);
webServo servoLeft(servoLeft_writePin);
webServo servoRight(servoRight_writePin);
webServo servoHand(servoHand_writePin);
WiFiServer server(80);

String header;


void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servoBottom.setup();
  servoLeft.setup();
  servoRight.setup();
  servoHand.setup();
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html; charset=UTF-8");
            client.println("Connection: close");
            client.println();

			// turns the servos on and off
			if (header.indexOf("GET /bottom/sweep") >= 0) { servoBottom.sweepServo(); Serial.println("Bottom servo sweeped");}
			if (header.indexOf("GET /left/sweep") >= 0) { servoLeft.sweepServo(); Serial.println("Left servo sweeped");}
			if (header.indexOf("GET /right/sweep") >= 0) { servoRight.sweepServo(); Serial.println("Right servo sweeped");}
			if (header.indexOf("GET /hand/sweep") >= 0) { servoHand.sweepServo(); Serial.println("Hand servo sweeped");}
			if (header.indexOf("GET /move/x-pos") >= 0) { servoRight.moveBy(10); Serial.println("Moved x + 10";}
			if (header.indexOf("GET /move/x-neg") >= 0) { servoRight.moveBy(-10); Serial.println("Moved x - 10");}
			if (header.indexOf("GET /move/y-pos") >= 0) { servoLeft.moveBy(10); Serial.println("Moved y + 10");}
			if (header.indexOf("GET /move/y-neg") >= 0) { servoLeft.moveBy(-10); Serial.println("Moved y - 10");}
			if (header.indexOf("GET /move/r-neg") >= 0) { servoBottom.moveBy(10); Serial.println("Rotated 10 degrees.");}
			if (header.indexOf("GET /move/r-pos") >= 0) { servoBottom.moveBy(-10); Serial.println("Rotated -10 degrees.");}
			if (header.indexOf("GET /move/h-neg") >= 0) { servoHand.moveBy(10); Serial.println("Hand opened by 10 degrees.");}
			if (header.indexOf("GET /move/h-pos") >= 0) { servoHand.moveBy(-10); Serial.println("Hand closed by 10 degrees.");}
			
			client.println("<!DOCTYPE html><html><head>");
			client.println("<title>Robot Arm Control</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
			client.println("<style>");
			client.println("html { font-family: Helvetica, Arial, sans-serif; text-align: center; background: linear-gradient(to bottom, #121212, #1e1e1e); color: white; }");
			client.println("h1 { color: #FF6F00; margin-bottom: 10px; }");
			client.println(".container { display: flex; justify-content: center; gap: 100px; padding: 30px; max-width: 900px; margin: auto; }");
			client.println(".column { display: flex; flex-direction: column; align-items: center; gap: 20px; background: #2a2a2a; padding: 20px; border-radius: 12px; box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.5); width: 300px; }");
			client.println(".button, .arrow-btn { background-color: #FF6F00; border: none; color: white; padding: 14px 36px; font-size: 20px; cursor: pointer; border-radius: 12px; transition: 0.3s; }");
			client.println(".button:hover, .arrow-btn:hover { background-color: #cc5500; transform: scale(1.05); }");
			client.println(".arrow-grid { display: grid; grid-template-columns: repeat(3, 60px); gap: 10px; justify-content: center; align-items: center; }");
			client.println(".arrow-btn { padding: 10px; font-size: 18px; width: 60px; height: 60px; background-color: #FF6F00; border-radius: 12px; transition: 0.3s; }");
			client.println(".arrow-btn:hover { background-color: #cc5500; }");
			client.println(".info-box { margin-top: 20px; padding: 15px; background: #333; border-radius: 10px; box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.3); width: 100%; max-width: 250px; text-align: left; color: white; }");
			client.println(".footer { margin-top: 20px; font-size: 14px; color: #aaa; }");
			client.println("</style></head><body>");

            client.println("<h1>ESP32 Robot Arm Control</h1>");
            client.println("<div class='container'>");
            
            client.println("<div class='column'>");
			// SWEEP BUTTONS
            client.println("<p>Bottom Servo</p>");
			client.println("<p><a href=\"/bottom/sweep\"><button class=\"button\">Sweep</button></a></p>");
            client.println("<p>Left Servo</p>");
			client.println("<p><a href=\"/left/sweep\"><button class=\"button\">Sweep</button></a></p>");
			client.println("<p>Right Servo</p>");
			client.println("<p><a href=\"/right/sweep\"><button class=\"button\">Sweep</button></a></p>"); 
			client.println("<p>Hand Servo</p>");
			client.println("<p><a href=\"/hand/sweep\"><button class=\"button\">Sweep</button></a></p>");
			//Manual control buttons
            client.println("</div>");
            client.println("<div class='column'>");
            client.println("<p><strong>Manual Control</strong></p>");
            client.println("<div class='arrow-grid'>");
            client.println("<a href='/move/r-neg'><button class='arrow-btn'>↺</button></a> <a href='/move/y-pos'><button class='arrow-btn'>▲</button></a> <a href='/move/r-pos'><button class='arrow-btn'>↻</button></a>");
            client.println("<a href='/move/x-neg'><button class='arrow-btn'>◄</button></a> <div></div> <a href='/move/x-pos'><button class='arrow-btn'>►</button></a>");
            client.println("<a href='/move/h-neg'><button class='arrow-btn'>❮</button></a> <a href='/move/y-neg'><button class='arrow-btn'>▼</button></a> <a href='/move/h-pos'><button class='arrow-btn'>❯</button></a>");
            client.println("</div>");
			// info box
            client.println("<div class='info-box'>");
            client.println("<p><strong>Servo Positions</strong></p>");
            client.println("<p>Bottom: " + String(servoBottom.getPosition()) + "°</p><p>Left: " + String(servoLeft.getPosition()) + "°</p><p>Right: " + String(servoRight.getPosition()) + "°</p><p>Hand: " + String(servoHand.getPosition()) + "°</p>");
            client.println("<hr><p><strong>Server IP:</strong> " + WiFi.softAPIP().toString() + "</p>");
            client.println("</div></div></div>");
            client.println("<footer class='footer'><p>Viktors Kočetoks, Filips Biete 12.EI</p></footer>");
            client.println("</body></html>");

            client.println();
            break;
          }
          currentLine = "";
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }
}
