// This example sends data to multiple variables to
// Ubidots through HTTP protocol.

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

// Create a pointer to a instance of the Ubidots class to use it globally
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
  ubidots = new Ubidots(UBIDOTS_TOKEN, UBI_HTTP);
  //ubidots->setDebug(true); // Uncomment this line for printing debug messages
}


void loop() {
  float value1 = random(0, 9) * 10;
  float value2 = random(0, 9) * 100;
  float value3 = random(0, 9) * 1000;
  ubidots->add("Variable_Name_One", value1);  // Change for your variable name
  ubidots->add("Variable_Name_Two", value2);
  ubidots->add("Variable_Name_Three", value3);

  bool bufferSent = false;
  bufferSent = ubidots->send();  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  delay(5000);
}
