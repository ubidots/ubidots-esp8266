#include "UbidotsMicroESP8266.h"

#define DEVICE_LABEL "..." // Put here your Device Label
#define VARIABLE_LABEL "..." // Put here your Variable Label
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

  float value = client.getValueWithDevice(DEVICE_LABEL, VARIABLE_LABEL);
  if (value != ERROR_VALUE){
    Serial.print("Value: ");
    Serial.println(value);
  } else {
    Serial.println("There was an error retrieving your data");
  }
  delay(10000);
}
