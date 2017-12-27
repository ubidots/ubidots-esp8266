#include "UbidotsMicroESP8266.h"

#define ID  "..."  // Put here your Ubidots variable ID
#define TOKEN  "..."  // Put here your Ubidots TOKEN
#define WIFISSID "..." // Put here your Wi-Fi SSID
#define PASSWORD "..." // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
    //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {
    long timestamp = client.getVarTimestamp(ID);
    if (timestamp != ERROR_VALUE){
      Serial.print(F("timestamp: "));
      Serial.println(timestamp);
    }else{
      Serial.println(F("Error getting timestamp"));
    }
    delay(1000);
}
