#include "WiFi.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Define Wifi Network and Password
const char* ssid = "****";
const char* password =  "****";

//Define Socket Port and Baud Rate
int port = 4999;
int baud = 9600;

//Define RX and TX GPIO
int RX_Pin = 39;
int TX_Pin = 32;

//Define GPS and Serial and cords
double X_Value;
double Y_Value;
TinyGPSPlus gps;
SoftwareSerial SerialGPS(RX_PIN, TX_PIN);

WiFiServer wifiServer(port);

void setup() {

  Serial.begin(baud);
  SerialGPS.begin(baud);
  delay(1000); //in mili sec

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Searching for Wifi");
  }

  Serial.println("Connected to Wifi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
  Serial.println("Ready for Client...");
  
}

void loop() {

  WiFiClient client = wifiServer.available();
  client.setNoDelay(true); //sends bytes immediately

  if (client) {

    while (client.connected()) {

      //If there is data in the buffer
      // 0 = Stop
	    // A = Travel to Site A
	    // B = Travel to Site B
	    // C = Travel to Site C
      while (client.available()>0) {
          char c = client.read();

          //Send this character to Arduino and ROS
          if((c != '\n') && (c != '\r')) Serial.println(c);      
      }
      
      //Read GPS
      while (SerialGPS.available() > 0) {
        if (gps.encode(SerialGPS.read())) {
          X_Value = (gps.location.lat(), 6);
          Y_Value = (gps.location.lng(), 6);
        }
      if (millis() > 5000 && gps.charsProcessed() < 10) {
          Serial.println("GPS NOT DETECTED!");
          X_Value = 0;
          Y_Value = 0;
        }
      
      //Send update to client
      //Make sure to send identifier, X or Y or L
      //Make sure to send '\n' to signal end of data

      // Xcord
      client.write("X");
      client.write(X_Value);
      client.write('\n');

      // Ycord
      client.write("Y");
      client.write(Y_Value);
      client.write('\n');

      //Check if its safe to launch

      
      //Launch Status
      client.write("L");
      client.write("1"); //1 means safe to launch
      client.write('\n');

      delay(5000);

    }

    client.stop();
    Serial.println("Client disconnected");
    Serial.println("Ready for Client...");

  }
}