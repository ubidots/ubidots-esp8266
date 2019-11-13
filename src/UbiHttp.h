/*
Copyright (c) 2013-2018 Ubidots.
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

Developed and maintained by Jose Garcia for IoT Services Inc
@jotathebest at github: https://github.com/jotathebest
*/

#ifndef _UbiHttp_H_
#define _UbiHttp_H_

#ifndef USING_AXTLS
#define USING_AXTLS
#endif

#include <ESP8266WiFi.h>
#include <WiFiClientSecureAxTLS.h>
#include <time.h>
#include "Arduino.h"
#include "UbiProtocol.h"
#include "stdint.h"

using namespace axTLS;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic pop

class UbiHTTP : public UbiProtocol {
 public:
  UbiHTTP(const char* host, const int port, const char* user_agent, const char* token);
  bool sendData(const char* device_label, const char* device_name, char* payload);
  float get(const char* device_label, const char* variable_label);
  void setDebug(bool debug);
  bool serverConnected();
  ~UbiHTTP();

 private:
  const char* _host;
  const char* _user_agent;
  const char* _token;
  int _port;
  bool _debug = false;
  bool waitServerAnswer();
  void reconnect(const char* host, int port);
  float parseHttpAnswer(const char* request_type, char* data);
  void readServerAnswer(char* response);
  int _timeout = 5000;
  uint8_t _maxReconnectAttempts = 5;
  bool _certifiedLoaded = false;
  bool _syncronizeTime();
  bool _loadCert();
  unsigned long _timerToSync = millis();
  bool _preConnectionChecks();
  WiFiClientSecure _client_https_ubi;
};

#endif
