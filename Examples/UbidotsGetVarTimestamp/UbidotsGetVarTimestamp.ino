#include "UbidotsMicroESP8266.h"
#define ID  "Your_VariableID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop() {
    long timestamp = client.getVarTimestamp(ID);
    Serial.print("timestamp: ");
    Serial.println(timestamp);
    delay(10000);
}