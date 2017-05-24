// This example is to save multiple variables to the Ubidots API with TCP method

/****************************************
 * Include Libraries
 ****************************************/

#include "UbidotsMicroESP8266.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password
#define VARIABLE_LABEL_1 "temperature" // Put Here  your variable label
//#define VARIABLE_LABEL_2 "humidity" // Put Here  your variable label

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/
void setup(){
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
    //client.setDebug(true); // Uncomment this line to set DEBUG on

}
void loop(){
    float value1 = analogRead(0);
    //float value2 = analogRead(2);
    client.add(VARIABLE_LABEL_1, value1); 
    //client.add(VARIABLE_LABEL_2, value2);
    client.sendAll(true);
    delay(5000);
}