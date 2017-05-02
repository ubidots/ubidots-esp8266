#include "UbidotsMicroESP8266.h"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
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
    client.add("temperature", value1);
    //client.add("switch", value2);
    client.sendAll(true);
    delay(5000);
}