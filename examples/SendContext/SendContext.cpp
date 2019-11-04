// This example sends data and context to a variable to
// Ubidots through TCP protocol.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Instances and Constants
 ****************************************/

const char* UBIDOTS_TOKEN = "...";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "...";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "...";      // Put here your Wi-Fi password
Ubidots ubidots(UBIDOTS_TOKEN, UBI_TCP);

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  // ubidots.setDebug(true); // Uncomment this line for printing debug messages
}

void loop() {
  float value = analogRead(A0);

  /* Adds context key-value pairs */
  ubidots.addContext("weather-status", "sunny");
  ubidots.addContext("time", "11:40:56 pm");

  /* Reserves memory to store context array */
  char* context = (char*)malloc(sizeof(char) * 60);

  /* Builds the context with the array above to send to Ubidots */
  ubidots.getContext(context);

  /* Sends the variable with the context */
  ubidots.add("temperature", value, context);  // Change for your variable name

  bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device ID

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  free(context);
  delay(5000);
}
