// This example send data with hardcoded GPS Coordinates
// to a variable to Ubidots through TCP protocol.

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
  // ubidots.setDebug(true); //Uncomment this line for printing debug messages
}

void loop() {
  float value = random(0, 9) * 10;

  /* Hardcoded Coordinates */
  float latitude = 37.773;
  float longitude = -6.2345;

  /* Reserves memory to store context key values, add as much as you need */
  char* str_lat = (char*)malloc(sizeof(char) * 10);
  char* str_lng = (char*)malloc(sizeof(char) * 10);

  /* Saves the coordinates as char */
  sprintf(str_lat, "%f", latitude);
  sprintf(str_lng, "%f", longitude);

  /* Reserves memory to store context array */
  char* context = (char*)malloc(sizeof(char) * 30);

  /* Adds context key-value pairs */
  ubidots.addContext("lat", str_lat);
  ubidots.addContext("lng", str_lng);

  /* Builds the context with the coordinates to send to Ubidots */
  ubidots.getContext(context);

  /* Sends the position */
  ubidots.add("position", value, context);  // Change for your variable name

  bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  /* free memory */
  free(str_lat);
  free(str_lng);
  free(context);
  delay(5000);
}
