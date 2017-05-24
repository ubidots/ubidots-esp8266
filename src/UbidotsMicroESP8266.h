/*
Copyright (c) 2013-2016 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Made by Mateo Velez - Metavix for Ubidots Inc
Modified by: Maria Carlina Hernandez

*/

#ifndef _UbidotsMicroESP8266_H_
#define _UbidotsMicroESP8266_H_
#endif

#include <ESP8266WiFi.h>
#include "WiFiUdp.h"

namespace {
    char* DEFAULT_DEVICE_NAME = "ESP8266";
    const char * TIME_SERVER = "pool.ntp.org";
    const char * SERVER = "translate.ubidots.com";
    const char * HTTPSERVER = "things.ubidots.com";
    const char * USER_AGENT = "UbidotsESP8266";
    const char * VERSION = "1.2";
    const float ERROR_VALUE = -3.4028235E+8;
    const int PORT = 9012;
    const int HTTPPORT = 80;
}

typedef struct Value {
  char  *id;
  char *context;
  float value_id;
  unsigned long timestamp;
} Value;

class Ubidots {
 public:
    explicit Ubidots(char* token, const char* server = SERVER);
    bool sendAll(bool type = false);
    bool sendHTTP();
    bool sendTLATE();
    float getValue(char* id);
    float getValueUDP(char *id);
    float getValueWithDevice(char* dsLabel, char* varLabel);
    long getVarTimestamp(char* id);
    char* getVarContext(char* id);
    void add(char *variable_label, float value);
    void add(char *variable_label, float value, char *ctext);
    void add(char *variable_label, float value, unsigned long timestamp);
    void add(char *variable_label, float value, char *ctext, unsigned long timestamp);
    void setDebug(bool debug);
    bool wifiConnection(char *ssid, char *pass);
    void setDataSourceName(char* dataSoruceName);
    void setDataSourceLabel(char* dataSoruceLabel);
    unsigned long ntpUnixTime ();

 private:
    bool _debug = false;
    char* _deviceName;
    char* _espID = (char *) malloc(sizeof(char) * 100);
    char* _token;
    const char* _server;
    float parseValue(String body);
    uint8_t currentValue;
    uint8_t maxValues;
    Value * val;
    void idAsMac();
    WiFiClient _client;
    WiFiUDP udp;
};
