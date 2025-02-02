#include <WiFi.h>

#define servoBottom_writePin 26
#define servoLeft_writePin 12
#define servoRight_writePin 14
#define servoHand_writePin 27

const char *ssid = "ENTER SSID";
const char *password = "ENTER PASSWORD";

NetworkServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(servoBottom_writePin, OUTPUT);
  pinMode(servoLeft_writePin, OUTPUT);
  pinMode(servoRight_writePin, OUTPUT);
  pinMode(servoHand_writePin, OUTPUT);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/bottomOn\">here</a> to start sweeping the bottom servo.<br>");
            client.print("Click <a href=\"/bottomOff\">here</a> to stop sweeping the bottom servo.<br>");
            client.print("Click <a href=\"/leftOn\">here</a> to start sweeping the left servo.<br>");
            client.print("Click <a href=\"/leftOff\">here</a> to stop sweeping the left servo.<br>");
            client.print("Click <a href=\"/rightOn\">here</a> to start sweeping the right servo.<br>");
            client.print("Click <a href=\"/rightOff\">here</a> to stop sweeping the right servo.<br>");
            client.print("Click <a href=\"/handOn\">here</a> to start sweeping the hand servo.<br>");
            client.print("Click <a href=\"/handOff\">here</a> to stop sweeping the hand servo.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /bottomOff")) {
          digitalWrite(servoBottom_writePin, LOW);
        }
        if (currentLine.endsWith("GET /bottomOn")) {
          digitalWrite(servoBottom_writePin, HIGH);
        }
        if (currentLine.endsWith("GET /leftOff")) {
          digitalWrite(servoLeft_writePin, LOW);
        }
        if (currentLine.endsWith("GET /leftOn")) {
          digitalWrite(servoLeft_writePin, HIGH);
        }
        if (currentLine.endsWith("GET /rightOff")) {
          digitalWrite(servoRight_writePin, LOW);
        }
        if (currentLine.endsWith("GET /rightOn")) {
          digitalWrite(servoRight_writePin, HIGH);
        }
        if (currentLine.endsWith("GET /handOff")) {
          digitalWrite(servoHand_writePin, LOW);
        }
        if (currentLine.endsWith("GET /handOn")) {
          digitalWrite(servoHand_writePin, HIGH);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
