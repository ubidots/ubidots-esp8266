
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
Developed and maintained by Jose Garcia and Cristian Arrieta for IoT Services
Inc
@jotathebest at github: https://github.com/jotathebest
@crisap94 at github: https://github.com/crisap94
*/

#include "Ubidots.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

Ubidots::Ubidots(const char *token, IotProtocol iotProtocol) { _builder(token, UBI_INDUSTRIAL, iotProtocol); }

Ubidots::Ubidots(const char *token, UbiServer server, IotProtocol iotProtocol) { _builder(token, server, iotProtocol); }

void Ubidots::_builder(const char *token, UbiServer server, IotProtocol iotProtocol) {
  _getDeviceMac(_defaultDeviceLabel);
  _iotProtocol = iotProtocol;
  _context = (ContextUbi *)malloc(MAX_VALUES * sizeof(ContextUbi));
  _cloudProtocol = new UbiProtocolHandler(token, server, iotProtocol);
}

/**************************************************************************
 * Destructor
 ***************************************************************************/

Ubidots::~Ubidots() {
  free(_context);
  delete _cloudProtocol;
}

/***************************************************************************
FUNCTIONS TO SEND DATA
***************************************************************************/

/**
 * Add a value of variable to save
 * @arg variable_label [Mandatory] variable label where the dot will be stored
 * @arg value [Mandatory] Dot value
 * @arg context [optional] Dot context to store. Default NULL
 * @arg dot_timestamp_seconds [optional] Dot timestamp in seconds, usefull for
 * datalogger. Default NULL
 * @arg dot_timestamp_millis [optional] Dot timestamp in millis to add to
 * dot_timestamp_seconds, usefull for datalogger.
 */

void Ubidots::add(const char *variable_label, double value) { add(variable_label, value, NULL, NULL, NULL); }

void Ubidots::add(const char *variable_label, double value, char *context) {
  add(variable_label, value, context, NULL, NULL);
}

void Ubidots::add(const char *variable_label, double value, char *context, long unsigned dot_timestamp_seconds) {
  add(variable_label, value, context, dot_timestamp_seconds, NULL);
}

void Ubidots::add(const char *variable_label, double value, char *context, long unsigned dot_timestamp_seconds,
                  unsigned int dot_timestamp_millis) {
  _cloudProtocol->add(variable_label, value, context, dot_timestamp_seconds, dot_timestamp_millis);
}

/**
 * Sends data to Ubidots
 * @arg device_label [Mandatory] device label where the dot will be stored
 * @arg device_name [optional] Name of the device to be created (supported only
 * for TCP/UDP)
 * @arg flags [Optional] Particle publish flags for webhooks
 */

bool Ubidots::send() { return send(_defaultDeviceLabel, _defaultDeviceLabel); }

bool Ubidots::send(const char *device_label) { return send(device_label, device_label); }

bool Ubidots::send(const char *device_label, const char *device_name) {
  if (strlen(_deviceType) > 0 && _iotProtocol == UBI_HTTP) {
    char builtDeviceLabel[50];
    sprintf(builtDeviceLabel, "%s/?type=%s", device_label, _deviceType);
    return _cloudProtocol->send(builtDeviceLabel, device_name);
  }
  return _cloudProtocol->send(device_label, device_name);
}

/***************************************************************************
AUXILIAR FUNCTIONS
***************************************************************************/

double Ubidots::get(const char *device_label, const char *variable_label) {
  return _cloudProtocol->get(device_label, variable_label);
}

void Ubidots::setDebug(bool debug) {
  _debug = debug;
  _cloudProtocol->setDebug(debug);
}

/*
 * Adds to the context structure values to retrieve later it easily by the user
 */

void Ubidots::addContext(char *key_label, char *key_value) {
  (_context + _current_context)->key_label = key_label;
  (_context + _current_context)->key_value = key_value;
  _current_context++;
  if (_current_context >= MAX_VALUES) {
    Serial.println(F("You are adding more than the maximum of consecutive "
                     "key-values pairs"));
    _current_context = MAX_VALUES;
  }
}

/*
 * Retrieves the actual stored context properly formatted
 */

void Ubidots::getContext(char *context_result) { getContext(context_result, _iotProtocol); }

void Ubidots::getContext(char *context_result, IotProtocol iotProtocol) {
  // TCP context type
  if (iotProtocol == UBI_TCP || iotProtocol == UBI_UDP) {
    sprintf(context_result, "");
    for (uint8_t i = 0; i < _current_context;) {
      sprintf(context_result, "%s%s=%s", context_result, (_context + i)->key_label, (_context + i)->key_value);
      i++;
      if (i < _current_context) {
        sprintf(context_result, "%s$", context_result);
      } else {
        sprintf(context_result, "%s", context_result);
        _current_context = 0;
      }
    }
  }

  // HTTP context type
  if (iotProtocol == UBI_HTTP) {
    sprintf(context_result, "");
    for (uint8_t i = 0; i < _current_context;) {
      sprintf(context_result, "%s\"%s\":\"%s\"", context_result, (_context + i)->key_label, (_context + i)->key_value);
      i++;
      if (i < _current_context) {
        sprintf(context_result, "%s,", context_result);
      } else {
        sprintf(context_result, "%s", context_result);
        _current_context = 0;
      }
    }
  }
}

bool Ubidots::wifiConnect(const char *ssid, const char *password) {
  WiFi.begin(ssid, password);
  uint8_t maxConnectionAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && maxConnectionAttempts < _maxConnectionAttempts) {
    delay(500);
    Serial.print(".");
    maxConnectionAttempts += 1;
  }
  if (WiFi.status() == WL_NO_SSID_AVAIL) {
    Serial.println("Your network SSID cannot be reached");
    return false;
  }
  if (WiFi.status() == WL_CONNECT_FAILED) {
    Serial.println("Network password incorrect");
    return false;
  }

  WiFi.setAutoReconnect(true);
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  return true;
}
bool Ubidots::wifiConnected() { return WiFi.status() != WL_CONNECTED; }
bool Ubidots::serverConnected() { return _cloudProtocol->serverConnected(); }

/* Obtains the device's MAC */
void Ubidots::_getDeviceMac(char macAddr[]) {
  byte mac[6];
  WiFi.macAddress(mac);
  sprintf(macAddr, "%.2X%.2X%.2X%.2X%.2X%.2X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/*
 * Makes available device types using HTTP
 */
void Ubidots::setDeviceType(const char *deviceType) {
  if (strlen(deviceType) > 0 && _iotProtocol == UBI_HTTP) {
    sprintf(_deviceType, "%s", deviceType);
  } else {
    Serial.println("Device Type is only available using HTTP");
  }
}
