#include "UbidotsMicroESP8266.h"

#define TOKEN  "...."  // Put here your Ubidots TOKEN
#define WIFISSID "...." // Put here your Wi-Fi SSID
#define PASSWORD "..." // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup(){
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  //client.setServerEndpoint("things.ubidots.com");  // Uncomment this line if you are an educational user
  //client.setDebug(true); // Uncomment this line to see debug messages
}
void loop(){
  float value1 = random(0, 1000)*1.0;
  float value2 = random(0, 1000)*1.0;
  client.add("temperature", value1);
  client.add("switch", value2);
  if (client.sendAll(true)) {
    Serial.println("Sucessfull!");
  } else {
    Serial.println("There was an error sending your data");
  }
  delay(5000);
}
