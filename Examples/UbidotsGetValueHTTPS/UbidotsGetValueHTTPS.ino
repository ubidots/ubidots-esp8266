#include "UbidotsMicroESP8266.h"

#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password


Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
    client.setDebug(true); // Uncomment this line to set DEBUG on
    client.setSSL(true);   // Activate SSL
}

void loop() {
    float value = client.getValue(ID);  //Now HTTPS
    Serial.print("Value: ");
    Serial.println(value);
    delay(10000);
}

