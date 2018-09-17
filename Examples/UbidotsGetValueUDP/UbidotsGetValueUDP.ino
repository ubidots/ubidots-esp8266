#include "UbidotsMicroESP8266.h"

#define ID  "..."  // Put here your Ubidots variable ID
#define TOKEN  "..."  // Put here your Ubidots TOKEN
#define WIFISSID "..." // Your SSID
#define PASSWORD "..." // Your Wi-Fi password

Ubidots client(TOKEN);


void setup() {
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  //client.setServerEndpoint("things.ubidots.com");  // Uncomment this line if you are an educational user
  //client.setDebug(true); // Uncomment this line to set DEBUG on

}

void loop() {

  float value = client.getValueUDP(ID);
  if (value != ERROR_VALUE){
    Serial.print("Value: ");
    Serial.println(value);
  } else {
    Serial.println("There was an error retrieving your data");
  }
  delay(10000);
}