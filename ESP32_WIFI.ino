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
char buffer[32];
int count = 0;
int launch = 0;
float X_Value = 28.6744496;
float Y_Value = -81.1834509;
TinyGPSPlus gps;
SoftwareSerial SerialGPS(RX_Pin, TX_Pin);

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
    
      while (SerialGPS.available() > 0) {
          if (gps.encode(SerialGPS.read())) {
              if (gps.location.isValid()) {
                  Serial.print("Latitude: ");
                  Serial.println(gps.location.lat());
                  X_Value = gps.location.lat();
                  Y_Value = gps.location.lng();
              }
              else {

                  Serial.println("Location is not available");

                  //Update GPS Example (for indoors when GPS doesent work)
                  /*
                  if(count == 0) {
                      X_Value = 28.6744496;
                      Y_Value = -81.1834509;
                      count = 1;
                      launch = 0;
                  }
                  else if (count == 1) {
                      X_Value = 28.6745246;
                      Y_Value = -81.1834009;
                      count = 2;
                      launch = 0;
                  }
                  else if (count == 2) {
                      X_Value = 28.6745996;
                      Y_Value = -81.1833509;
                      count = 0;
                      launch = 1;
                  }
                  */
                  
                     
              }
          }
      }
      if (millis() > 5000 && gps.charsProcessed() < 10) Serial.println("GPS NOT DETECTED!");

      //Send update to client
      //Make sure to send identifier, X or Y or L
      //Make sure to send '\n' to signal end of data

      // Xcord
      snprintf(buffer, sizeof(buffer), "%.6f", X_Value);
      client.write("X");
      client.write(buffer);
      client.write('\n');

      // Ycord
      snprintf(buffer, sizeof(buffer), "%.6f", Y_Value);
      client.write("Y");
      client.write(buffer);
      client.write('\n');
      
      //Launch Status
      snprintf(buffer, sizeof(buffer), "%d", launch);
      client.write("L");
      client.write(buffer); //1 means safe to launch
      client.write('\n');

      delay(5000);

    }

    client.stop();
    Serial.println("Client disconnected");
    Serial.println("Ready for Client...");

  }
}
