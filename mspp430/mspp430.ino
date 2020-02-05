#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
long now = millis();
long lastMeasure = 0;
// your network name also called SSID
char ssid[] = "dummy";
// your network password
char password[] = "lmfaohaha";
// MQTTServer to use
char server[] = "192.168.43.45";
byte buffer[128];
void callback(char*,byte*,unsigned int);
WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

 void callback(char* topic, byte* payload, unsigned int length) {
   Serial.println("Got a message, publishing it");
   // If length is larger than 128,
   // then make sure we do not write more then our buffer can handle
   if(length > 128) length = 128;
  
   memcpy(buffer, payload, length);
   client.publish("outTopic", buffer, length);
 }

void setup()
{
  Serial.begin(115200);
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  // Print the WiFi status.
  printWifiStatus();
}

int tem, vib;
char tempstring[8], vibstring[8];

void loop()
{
  // Reconnect if the connection was lost
  if (!client.connected()) {
    Serial.println("Disconnected. Reconnecting....");
    if(!client.connect("energiaClient")) {
      Serial.println("Connection failed");
    } 
    else {
      Serial.println("Connection success");
    }
  }
  tem=analogRead(A0);
  vib=analogRead(A1);
  Serial.print("temp is: ");
  Serial.println(tem);
  Serial.print("vib is: ");
  Serial.println(vib);
  itoa(tem,tempstring,10);
  itoa(vib,vibstring,10);
  client.publish("/motor1/temp",tempstring);
  delay(500);
  client.publish("/motor1/vib",vibstring);
  // Check if any message were received on the topic we subsrcibed to
  client.poll();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}