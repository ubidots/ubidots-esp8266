// This example sends data along with timestamp in POSIX ms standard
// to a variable to Ubidots API through HTTP, TCP or UDP protocol.

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
const char* VARIABLE_LABEL = "..."; // Put here your variable label

Ubidots* ubidots{nullptr};

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  Ubidots::wifiConnect(WIFI_SSID, WIFI_PASS);
  ubidots = new Ubidots(UBIDOTS_TOKEN);
  //ubidots->setDebug(true); // Uncomment this line for printing debug messages
}

void loop() {
  float value1 = analogRead(A0);
  unsigned long timestamp_seconds = 1704469763L;  // Put here your timestamp in seconds
  unsigned int timestamp_milliseconds = 0;        // Put here the number of milliseconds to shift your timestamp

  ubidots->add(VARIABLE_LABEL, value1, NULL, timestamp_seconds, timestamp_milliseconds);  // Change for your variable name

  bool bufferSent = false;
  bufferSent = ubidots->send();  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  delay(5000);
}
