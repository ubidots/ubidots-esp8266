#include "UbidotsMicroESP8266.h"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define ID_1 "Your_variable_ID_here" // Put your variable ID here
//#define ID_2 "Your_variable_ID_here" // Put your variable ID here
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
    //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop(){
    float value1 = analogRead(0);
    //float value2 = analogRead(2)
    client.add(ID_1, value1);
    //client.add(ID_2, value2);
    client.sendAll(false);
    delay(5000);
}