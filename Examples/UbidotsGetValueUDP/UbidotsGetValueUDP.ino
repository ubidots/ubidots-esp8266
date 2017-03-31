#include "UbidotsMicroESP8266.h"

#define ID  "..."  // Put here your Ubidots variable ID
#define TOKEN  "..."  // Put here your Ubidots TOKEN
#define WIFISSID "..." // Your SSID
#define PASSWORD "..." // Your Wi-Fi password

Ubidots client(TOKEN);


void setup() {
  Serial.begin(115200);
  delay(10);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {

  float value = client.getValueUDP(ID);
  Serial.print("Value: ");
  Serial.println(value);
  delay(10000);
}