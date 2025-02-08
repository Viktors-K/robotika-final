#include <WiFi.h>

#define servoBottom_writePin 26
#define servoLeft_writePin 12
#define servoRight_writePin 14
#define servoHand_writePin 27

const char* ssid     = "Robot_Arm";
const char* password = "Beet302";


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

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /bottom/on") >= 0) {
              Serial.println("Bottom servo on");
              servoBottom_State = "on";
              digitalWrite(servoBottom_writePin, HIGH);
            } else if (header.indexOf("GET /bottom/off") >= 0) {
              Serial.println("Bottom servo off");
              servoBottom_State = "off";
              digitalWrite(servoBottom_writePin, LOW);
            } else if (header.indexOf("GET /left/on") >= 0) {
              Serial.println("Left servo on");
              servoLeft_State = "on";
              digitalWrite(servoLeft_writePin, HIGH);
            } else if (header.indexOf("GET /left/off") >= 0) {
              Serial.println("Left servo off");
              servoLeft_State = "off";
              digitalWrite(servoLeft_writePin, LOW);
            } else if (header.indexOf("GET /right/on") >= 0) {
              Serial.println("Right servo on");
              servoRight_State = "on";
              digitalWrite(servoRight_writePin, HIGH);
            } else if (header.indexOf("GET /right/off") >= 0) {
              Serial.println("Right servo off");
              servoRight_State = "off";
              digitalWrite(servoRight_writePin, LOW);
            } else if (header.indexOf("GET /hand/on") >= 0) {
              Serial.println("Hand servo on");
              servoHand_State = "on";
              digitalWrite(servoHand_writePin, HIGH);
            } else if (header.indexOf("GET /hand/off") >= 0) {
              Serial.println("Hand servo off");
              servoHand_State = "off";
              digitalWrite(servoHand_writePin, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Servo Web Server</h1>");
            
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

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
