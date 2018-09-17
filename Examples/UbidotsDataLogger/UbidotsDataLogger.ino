#include "UbidotsMicroESP8266.h"

#define TOKEN  "...."  // Put here your Ubidots TOKEN
#define WIFISSID "...." // Put here your Wi-Fi SSID
#define PASSWORD "...." // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup(){
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  //client.setDebug(true); // Uncomment this line to see debug messages
  //client.setServerEndpoint("things.ubidots.com");  // Uncomment this line if you are an educational user
}
void loop(){
  float value1 = random(0, 1000)*1.0;
  /**********************************
  Uncomment this block if you wish to
  send data through HTTP
  **********************************/
  /*
  unsigned long timestamp = 1536610363;  // Timestamp in seconds

  client.add("temperature", value1, NULL, timestamp);
  
  if (client.sendAll(false)) {
    Serial.println("Sucessfull!");
  } else {
    Serial.println("There was an error sending your data");
  }
  */

  /**********************************
  End of HTTP Block Example
  **********************************/

  /* ------------------------------------ */

  /**********************************
  Uncomment this block if you wish to
  send data through TCP
  **********************************/
  unsigned long timestamp = 1536610363;  // Timestamp in seconds

  client.add("temperature", value1, NULL, NULL);
  
  if (client.sendAll(true)) {
    Serial.println("\nSucessfull!");
  } else {
    Serial.println("\nThere was an error sending your data");
  }

  /**********************************
  End of TCP Block Example
  **********************************/
  
  delay(5000);
}

