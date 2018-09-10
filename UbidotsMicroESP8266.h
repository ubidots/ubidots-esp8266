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
  const char * DEFAULT_DEVICE_NAME = "arduino-ethernet";
  const char * UBIDOTS_HTTP_SERVER = "industrial.api.ubidots.com";
  const char * UBIDOTS_TCP_SERVER = "industrial.api.ubidots.com";
  const char * USER_AGENT = "UbidotsESP8266";
  const char * VERSION = "1.4";
  const int UBIDOTS_HTTP_PORT = 80;
  const int UBIDOTS_TCP_PORT = 9012;
  const char * TIME_SERVER = "pool.ntp.org";
  const float ERROR_VALUE = -3.4028235E+8;
}

typedef struct Value {
  char  *id;
  char *context;
  float value_id;
  unsigned long timestamp;
} Value;

class Ubidots {
 public:
    Ubidots(char* token, const char * server = UBIDOTS_TCP_SERVER);
    bool sendAll(bool type = false);
    bool sendHTTP();
    bool sendTLATE();
    float getValue(char *id);
    float getValueUDP(char *id);
    float getValueWithDevice(char* dsLabel, char* varLabel);
    long getVarTimestamp(char* id);
    char* getVarContext(char* id);
    void add(char *variable_id, float value);
    void add(char *variable_id, float value, char *ctext);
    void add(char *variable_id, float value, unsigned long timestamp);
    void add(char *variable_id, float value, char *ctext, unsigned long timestamp);
    unsigned long ntpUnixTime ();
    void setDebug(bool debug);
    void setDataSourceName(char* dataSoruceName);
    void setDataSourceLabel(char* dataSoruceLabel);
    void setServerEndpoint(const char * server);
    bool wifiConnection(char *ssid, char *pass);

 private:
    void idAsMac();
    bool _debug = false;
    char* _token;
    const char * _server;
    const char * _dsName;
    char* _idName;
    char* _espID = (char *) malloc(sizeof(char) * 100);
    char* _context = (char *) malloc(sizeof(char) * 100);
    uint8_t _maxValues;
    uint8_t _currentValue;
    Value * val;
    float parseValue(String body);
    void createHttpPayload(char* payload);
    void readServerResponse();
    WiFiClient _client;
    WiFiUDP _udp;
};
