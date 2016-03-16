#include "UbidotsMicroESP8266.h"
#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define ID2 "Your_variable_ID2_here"
#define ID3 "Your_variable_ID3_here"
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN


Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    float value = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    client.add(ID, value);
    client.add(ID2, value2);
    client.add(ID3, value3);
    client.sendAll();
}
