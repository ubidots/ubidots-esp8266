#ifndef _UbidotsMicroESP8266_H_
#define _UbidotsMicroESP8266_H_

#include <ESP8266WiFi.h>

#define SERVER "things.ubidots.com"
#define PORT 80

typedef struct Value {
  char  *id;
  float value_id;
} Value;

class Ubidots {
   public:
      Ubidots(char* token);
      bool sendAll();
      float getValue(char* id);
      void add(char *variable_id, double value);
      bool wifiConnection(char *ssid, char *pass);
   
   private:
      char* _token;
      uint8_t maxValues;
      uint8_t currentValue;
      Value * val;
      float parseValue(String body);
      WiFiClient _client; 
};

#endif