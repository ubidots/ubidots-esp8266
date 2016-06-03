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

*/

#include "UbidotsMicroESP8266.h"
/**
 * Constructor.
 */
Ubidots::Ubidots(char* token, char* server) {
    _token = token;
    _server = server;
    _dsTag = "ESP8266";
    _dsName = "ESP8266";
    maxValues = 5;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
}
void Ubidots::setDataSourceName(char *dataSourceName) {
    _dsName = dataSourceName;
}
void Ubidots::setDataSourceLabel(char *dataSourceLabel) {
    _dsTag = dataSourceLabel;
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id) {
  float num;
  String response;
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  if (_client.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Geting your variable"));
        _client.print(F("GET /api/v1.6/variables/"));
        _client.print(id);
        _client.println(F("/values?page_size=1 HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: Arduino-Ethernet/1.0"));
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println();
  } else {
        return NULL;
  }
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        response = _client.readString();
    }
    Serial.println(response);
    bodyPosinit = 4 + response.indexOf("\r\n\r\n");
    response = response.substring(bodyPosinit);
    Serial.println(response);
    bodyPosinit = 9 + response.indexOf("\"value\":");
    bodyPosend = response.indexOf(", \"timestamp\"");
    num = response.substring(bodyPosinit,bodyPosend).toFloat();
    _client.flush();
    _client.stop();
    return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, float value, char *ctext) {
    (val+currentValue)->id = variable_id;
    (val+currentValue)->value_id = value;
    (val+currentValue)->context = ctext;
    currentValue++;
    if (currentValue>maxValues) {
        Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
        currentValue = maxValues;
    }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(bool type) {
    if (type) {
        return sendTLATE();
    } else {
        return sendHTTP();
    }
}
bool Ubidots::sendTLATE() {
    uint8_t i;
    String str;
    char data[256];
    sprintf(data, "=>");
    for (i = 0; i < currentValue; i++) {
        str = String(((val+i)->value_id), 5);
        sprintf(data, "%s%s:%s", data, (val+i)->id, str.c_str());
        if ((val+currentValue)->context) {
            sprintf(data, "%s$%s", data, (val+i)->context);
        }
        sprintf(data, "%s,", data);
    }
    Serial.println(data);
    if (_client.connect(SERVER, PORT)) {
        Serial.println(F("POSTING YOUR VARIABLES"));
        _client.print(USER_AGENT);
        _client.print(F("/"));
        _client.print(VERSION);
        _client.print(F("|POST|"));
        _client.print(_token);
        _client.print(F("|"));
        _client.print(_dsTag);
        _client.print(F(":"));
        _client.print(_dsName);
        _client.print(data);
        _client.println(F("|end"));
    } else {
        Serial.println(F("Connection to server failed"));
        currentValue = 0;
        return false;
    }
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    return true;
}
bool Ubidots::sendHTTP() {
    uint16_t i;
    String all;
    String str;
    all = "[";
    for (i = 0; i < currentValue; ) {
        str = String(((val+i)->value_id), 4);
        all += "{\"variable\": \"";
        all += String((val + i)->id);
        all += "\", \"value\":";
        all += str;
        all += "}";
        i++;
        if (i < currentValue) {
            all += ", "; 
        }
    }
    all += "]";
    i = all.length();
    if (_client.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Posting your variables"));
        _client.println(F("POST /api/v1.6/collections/values/?force=true HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: Arduino-Ethernet/1.0"));
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println(F("Content-Type: application/json"));
        _client.print(F("Content-Length: "));
        _client.println(String(i));
        _client.println();
        _client.println(all);
        _client.println();
    }
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    _client.stop();
    return true;
}
bool Ubidots::wifiConnection(char* ssid, char* pass) {
    WiFi.begin(ssid, pass);
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
        delay(500);
        Serial.print(".");
        timeout++;
    }
    Serial.println(F("WiFi connected"));  
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());
}