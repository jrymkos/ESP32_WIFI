#include "WiFi.h"

//Define Wifi Network and Password
const char* ssid = "****";
const char* password =  "****";

//Define Socket Port and Baud Rate
int port = 4999;
int baud = 115200;

WiFiServer wifiServer(port);

void setup() {

  Serial.begin(baud);
  WiFi.softAP("Senior Design"); //setup access point
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

  WiFi.softAP("Senior Design"); //setup access point
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
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

      //Send update to client
      //Make sure to send identifier, X or Y
      //Make sure to send '\n' to signal end of data
      
      // Xcord
      client.write("X");
      client.write("43.837589");
      client.write('\n');

      // Ycord
      client.write("Y");
      client.write("82.146762");
      client.write('\n');

      delay(5000);

    }

    client.stop();
    Serial.println("Client disconnected");
    Serial.println("Ready for Client...");

  }
}