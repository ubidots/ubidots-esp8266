#include "UbidotsMicroESP8266.h"

#define TOKEN  "...."  // Put here your Ubidots TOKEN
#define WIFISSID "...." // Put here your Wi-Fi SSID
#define PASSWORD "..." // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup(){
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  client.setDebug(true); // Uncomment this line to see debug messages
}
void loop(){
  float value1 = random(0, 1000)*1.0;
  /**********************************
  Uncomment this block if you wish to
  send data through HTTP
  **********************************/
  client.setServerEndpoint("things.ubidots.com");  // Uncomment this line if you are an educational user
  char str_lat[30];
  char str_lng[30];
  float lat = -6.2056;
  float lng = 70.236;

  // 5 is mininum width, 2 is precision; float value is copied onto str_val
  dtostrf(lat, 5, 2, str_lat);
  dtostrf(lng, 5, 2, str_lng);

  char context[100];
  sprintf(context, "{\"lat\":%s, \"lng\":%s}", str_lat, str_lng);
  client.add("position", value1, context);
  
  if (client.sendAll(false)) {
    Serial.println("Sucessfull!");
  } else {
    Serial.println("There was an error sending your data");
  }

  /**********************************
  End of HTTP Block Example
  **********************************/

  /* ------------------------------------ */

  /**********************************
  Uncomment this block if you wish to
  send data through TCP
  **********************************/
  /*
  client.setServerEndpoint("translate.ubidots.com");  // Uncomment this line if you are an educational user
  char str_lat[30];
  char str_lng[30];
  float lat = -6.2056;
  float lng = 70.236;

  // 5 is mininum width, 2 is precision; float value is copied onto str_val
  dtostrf(lat, 5, 2, str_lat);
  dtostrf(lng, 5, 2, str_lng);

  char context[100];
  sprintf(context, "\"lat\"=%s$\"lng\"=%s}", str_lat, str_lng);
  client.add("position", value1, context);
  
  if (client.sendAll(true)) {
    Serial.println("\nSucessfull!");
  } else {
    Serial.println("\nThere was an error sending your data");
  }*/

  /**********************************
  End of TCP Block Example
  **********************************/
  
  delay(5000);
}

