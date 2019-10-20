// This example retrieves last value of a variable from the Ubidots API
// using TCP protocol.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

const char* UBIDOTS_TOKEN = "...";  // Put here your Ubidots TOKEN
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
  // ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
  /* Obtain last value from a variable as float using TCP */
  float value = ubidots.get("weather-station", "temperature");

  // Evaluates the results obtained
  if (value != ERROR_VALUE) {
    Serial.print("Value:");
    Serial.println(value);
  }
  delay(5000);
}
