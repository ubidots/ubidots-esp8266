// This example is to get the last value of a variable from the Ubidots API

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsMicroESP8266.h"

/****************************************
 * Define Constants
 ****************************************/
#define DEVICE_LABEL "..." // Put here your Device Label
#define VARIABLE_LABEL "..." // Put here your Variable Label
#define TOKEN  "..."  // Put here your Ubidots TOKEN
#define WIFISSID "..." // Your SSID
#define PASSWORD "..." // Your Wi-Fi password

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {

  float value = client.getValueWithDevice(DEVICE_LABEL, VARIABLE_LABEL);
  Serial.print("Value: ");
  Serial.println(value);
  delay(10000);
}
