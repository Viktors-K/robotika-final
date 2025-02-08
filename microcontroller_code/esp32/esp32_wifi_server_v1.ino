#include <WiFi.h>

#define servoBottom_writePin 26
#define servoLeft_writePin 12
#define servoRight_writePin 14
#define servoHand_writePin 27

const char* ssid = "Robot_Arm";
const char* password = "Biete302";


WiFiServer server(80);

String header;
String servoBottom_State = "off";
String servoLeft_State = "off";
String servoRight_State = "off";
String servoHand_State = "off";

void setup() {
  Serial.begin(115200);
  pinMode(servoBottom_writePin, OUTPUT);
  pinMode(servoLeft_writePin, OUTPUT);
  pinMode(servoRight_writePin, OUTPUT);
  pinMode(servoHand_writePin, OUTPUT);
  digitalWrite(servoBottom_writePin, LOW);
  digitalWrite(servoLeft_writePin, LOW);
  digitalWrite(servoRight_writePin, LOW);
  digitalWrite(servoHand_writePin, LOW);
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
            if (header.indexOf("GET /bottom/on") >= 0) { digitalWrite(servoBottom_writePin, HIGH); servoBottom_State = "on"; Serial.println("Bottom servo on");}
            if (header.indexOf("GET /bottom/off") >= 0) { digitalWrite(servoBottom_writePin, LOW); servoBottom_State = "off"; Serial.println("Bottom servo off");}
            if (header.indexOf("GET /left/on") >= 0) { digitalWrite(servoLeft_writePin, HIGH); servoLeft_State = "on"; Serial.println("Left servo on");}
            if (header.indexOf("GET /left/off") >= 0) { digitalWrite(servoLeft_writePin, LOW); servoLeft_State = "off"; Serial.println("Left servo off");}
            if (header.indexOf("GET /right/on") >= 0) { digitalWrite(servoRight_writePin, HIGH); servoRight_State = "on"; Serial.println("Right servo on");}
            if (header.indexOf("GET /right/off") >= 0) { digitalWrite(servoRight_writePin, LOW); servoRight_State = "off"; Serial.println("Right servo off");}
            if (header.indexOf("GET /hand/on") >= 0) { digitalWrite(servoHand_writePin, HIGH); servoHand_State = "on"; Serial.println("Hand servo on");}
            if (header.indexOf("GET /hand/off") >= 0) { digitalWrite(servoHand_writePin, LOW); servoHand_State = "off"; Serial.println("Hand servo off");}

			client.println("<!DOCTYPE html><html><head>");
			client.println("<title>Robot Arm Control</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
			client.println("<style>");
			client.println("html { font-family: Helvetica, Arial, sans-serif; text-align: center; background: linear-gradient(to bottom, #121212, #1e1e1e); color: white; }");
			client.println("h1 { color: #FF6F00; margin-bottom: 10px; }");
			client.println(".container { display: flex; justify-content: center; gap: 100px; padding: 30px; max-width: 900px; margin: auto; }");
			client.println(".column { display: flex; flex-direction: column; align-items: center; gap: 20px; background: #2a2a2a; padding: 20px; border-radius: 12px; box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.5); width: 300px; }");
			client.println(".button, .arrow-btn { background-color: #FF6F00; border: none; color: white; padding: 14px 36px; font-size: 20px; cursor: pointer; border-radius: 12px; transition: 0.3s; }");
			client.println(".button:hover, .arrow-btn:hover { background-color: #cc5500; transform: scale(1.05); }");
			client.println(".button2 { background-color: #444; }");
			client.println(".button2:hover { background-color: #666; }");
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
			// Display current state, and ON/OFF buttons for bottom servo  
            client.println("<p>Bottom Servo - State " + servoBottom_State + "</p>");
            // If the servoBottom_State is off, it displays the ON button       
            if (servoBottom_State=="off") {
              client.println("<p><a href=\"/bottom/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/bottom/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for left servo  
            client.println("<p>Left Servo - State " + servoLeft_State + "</p>");
            // If the servoLeft_State is off, it displays the ON button       
            if (servoLeft_State=="off") {
              client.println("<p><a href=\"/left/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/left/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
			// Display current state, and ON/OFF buttons for right servo  
			client.println("<p>Right Servo - State " + servoRight_State + "</p>");
			// If the servoRight_State is off, it displays the ON button       
			if (servoRight_State == "off") {
				client.println("<p><a href=\"/right/on\"><button class=\"button\">ON</button></a></p>");
			} else {
				client.println("<p><a href=\"/right/off\"><button class=\"button button2\">OFF</button></a></p>");
			} 

			// Display current state, and ON/OFF buttons for hand servo  
			client.println("<p>Hand Servo - State " + servoHand_State + "</p>");
			// If the servoHand_State is off, it displays the ON button       
			if (servoHand_State == "off") {
				client.println("<p><a href=\"/hand/on\"><button class=\"button\">ON</button></a></p>");
			} else {
				client.println("<p><a href=\"/hand/off\"><button class=\"button button2\">OFF</button></a></p>");
			}

            client.println("</div>");
            client.println("<div class='column'>");
            client.println("<p><strong>Manual Control</strong></p>");
            client.println("<div class='arrow-grid'>");
            client.println("<div></div> <a href='/move/up'><button class='arrow-btn'>▲</button></a> <div></div>");
            client.println("<a href='/move/left'><button class='arrow-btn'>◄</button></a> <div></div> <a href='/move/right'><button class='arrow-btn'>►</button></a>");
            client.println("<div></div> <a href='/move/down'><button class='arrow-btn'>▼</button></a> <div></div>");
            client.println("</div>");
            client.println("<div class='info-box'>");
            client.println("<p><strong>Servo Positions</strong></p>");
            client.println("<p>Bottom: 0°</p><p>Left: 0°</p><p>Right: 0°</p><p>Hand: 0°</p>");
            client.println("<hr><p><strong>Server IP:</strong> " + WiFi.softAPIP().toString() + "</p>");
            client.println("</div></div></div>");
            client.println("<footer class='footer'><p>Filips Biete, Viktors Kočetoks 12.EI</p></footer>");
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
