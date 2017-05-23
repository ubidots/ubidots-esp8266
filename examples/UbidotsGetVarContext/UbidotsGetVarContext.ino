// This example is to get the context of a variable from the Ubidots API

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsMicroESP8266.h"
#include "UbidotsMicroESP8266.h"

/****************************************
 * Define Constants
 ****************************************/
#define ID  "Your_VariableID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

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
    char* context = client.getVarContext(ID);
    Serial.print("context: ");
    Serial.println(context);    
    delay(10000);
}
