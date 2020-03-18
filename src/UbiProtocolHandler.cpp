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

#include "UbiProtocolHandler.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

UbiProtocolHandler::UbiProtocolHandler(const char *token, IotProtocol iot_protocol) {
  _builder(token, UBI_INDUSTRIAL, iot_protocol);
}

UbiProtocolHandler::UbiProtocolHandler(const char *token, UbiServer server, IotProtocol iot_protocol) {
  _builder(token, server, iot_protocol);
}

void UbiProtocolHandler::_builder(const char *token, UbiServer server, IotProtocol iot_protocol) {
  _iot_protocol = iot_protocol;
  UbiBuilder builder(server, token, _iot_protocol);
  _dots = (Value *)malloc(MAX_VALUES * sizeof(Value));
  _ubiProtocol = builder.builder();
  _token = token;
  _current_value = 0;
}

/**************************************************************************
 * Overloaded destructor
 ***************************************************************************/

UbiProtocolHandler::~UbiProtocolHandler() {
  free(_dots);
  delete _ubiProtocol;
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

void UbiProtocolHandler::add(const char *variable_label, float value, char *context,
                             unsigned long dot_timestamp_seconds, unsigned int dot_timestamp_millis) {
  _dirty = true;
  (_dots + _current_value)->variable_label = variable_label;
  (_dots + _current_value)->dot_value = value;
  (_dots + _current_value)->dot_context = context;
  (_dots + _current_value)->dot_timestamp_seconds = dot_timestamp_seconds;
  (_dots + _current_value)->dot_timestamp_millis = dot_timestamp_millis;
  _current_value++;
  if (_current_value > MAX_VALUES) {
    if (_debug) {
      Serial.println(F("You are sending more than the maximum of consecutive variables"));
    }
    _current_value = MAX_VALUES;
  }
}

/**
 * Sends data to Ubidots
 * @arg device_label [Mandatory] device label where the dot will be stored
 * @arg device_name [optional] Name of the device to be created (supported only
 * for TCP/UDP)
 */

bool UbiProtocolHandler::send(const char *device_label, const char *device_name) {
  // Builds the payload
  char *payload = (char *)malloc(sizeof(char) * MAX_BUFFER_SIZE);
  if (_iot_protocol == UBI_TCP || _iot_protocol == UBI_UDP) {
    buildTcpPayload(payload, device_label, device_name);
  } else {
    buildHttpPayload(payload);
  }

  // Sends data
  if (_debug) {
    Serial.println("Sending data...");
  }

  bool result = _ubiProtocol->sendData(device_label, device_name, payload);
  free(payload);
  if (result) {
    _dirty = false;
    _current_value = 0;
  }

  _current_value = 0;
  return result;
}

double UbiProtocolHandler::get(const char *device_label, const char *variable_label) {
  if (_iot_protocol == UBI_UDP) {
    Serial.println("ERROR, data retrieval is only supported using TCP or HTTP protocols");
    return ERROR_VALUE;
  }

  double value = ERROR_VALUE;

  value = _ubiProtocol->get(device_label, variable_label);

  return value;
}

/**
 * Builds the HTTP payload to send and saves it to the input char pointer.
 * @payload [Mandatory] char payload pointer to store the built structure.
 * @timestamp_global [Optional] If set, it will be used for any dot without
 * timestamp.
 */

void UbiProtocolHandler::buildHttpPayload(char *payload) {
  /* Builds the payload */
  sprintf(payload, "{");

  for (uint8_t i = 0; i < _current_value;) {
    char str_value[20];
    _floatToChar(str_value, (_dots + i)->dot_value);
    sprintf(payload, "%s\"%s\":{\"value\":%s", payload, (_dots + i)->variable_label, str_value);

    // Adds timestamp seconds
    if ((_dots + i)->dot_timestamp_seconds != NULL) {
      sprintf(payload, "%s,\"timestamp\":%lu", payload, (_dots + i)->dot_timestamp_seconds);
      // Adds timestamp milliseconds
      if ((_dots + i)->dot_timestamp_millis != NULL) {
        char milliseconds[3];
        int timestamp_millis = (_dots + i)->dot_timestamp_millis;
        uint8_t units = timestamp_millis % 10;
        uint8_t dec = (timestamp_millis / 10) % 10;
        uint8_t hund = (timestamp_millis / 100) % 10;
        sprintf(milliseconds, "%d%d%d", hund, dec, units);
        sprintf(payload, "%s%s", payload, milliseconds);
      } else {
        sprintf(payload, "%s000", payload);
      }
    }

    // Adds dot context
    if ((_dots + i)->dot_context != NULL) {
      sprintf(payload, "%s,\"context\": {%s}", payload, (_dots + i)->dot_context);
    }

    sprintf(payload, "%s}", payload);
    i++;

    if (i < _current_value) {
      sprintf(payload, "%s,", payload);
    } else {
      sprintf(payload, "%s}", payload);
      _current_value = 0;
    }
  }

  if (_debug) {
    Serial.println("----------");
    Serial.println("payload:");
    Serial.println(payload);
    Serial.println("----------");
    Serial.println("");
  }
}

/**
 * Builds the TCP payload to send and saves it to the input char pointer.
 * @payload [Mandatory] char payload pointer to store the built structure.
 * @timestamp_global [Optional] If set, it will be used for any dot without
 * timestamp.
 */

void UbiProtocolHandler::buildTcpPayload(char *payload, const char *device_label, const char *device_name) {
  sprintf(payload, "");
  sprintf(payload, "%s|POST|%s|", USER_AGENT, _token);
  sprintf(payload, "%s%s:%s", payload, device_label, device_name);

  sprintf(payload, "%s=>", payload);
  for (uint8_t i = 0; i < _current_value;) {
    char str_value[20];
    _floatToChar(str_value, (_dots + i)->dot_value);
    sprintf(payload, "%s%s:%s", payload, (_dots + i)->variable_label, str_value);

    // Adds dot context
    if ((_dots + i)->dot_context != NULL) {
      sprintf(payload, "%s$%s", payload, (_dots + i)->dot_context);
    }

    // Adds timestamp seconds
    if ((_dots + i)->dot_timestamp_seconds != NULL) {
      sprintf(payload, "%s@%lu", payload, (_dots + i)->dot_timestamp_seconds);
      // Adds timestamp milliseconds
      if ((_dots + i)->dot_timestamp_millis != NULL) {
        char milliseconds[3];
        int timestamp_millis = (_dots + i)->dot_timestamp_millis;
        uint8_t units = timestamp_millis % 10;
        uint8_t dec = (timestamp_millis / 10) % 10;
        uint8_t hund = (timestamp_millis / 100) % 10;
        sprintf(milliseconds, "%d%d%d", hund, dec, units);
        sprintf(payload, "%s%s", payload, milliseconds);
      } else {
        sprintf(payload, "%s000", payload);
      }
    }

    i++;

    if (i < _current_value) {
      sprintf(payload, "%s,", payload);
    } else {
      sprintf(payload, "%s|end", payload);
      _current_value = 0;
    }
  }

  if (_debug) {
    Serial.println("----------");
    Serial.println("payload:");
    Serial.println(payload);
    Serial.println("----------");
    Serial.println("");
  }
}

/*
  Makes debug messages available
*/

void UbiProtocolHandler::setDebug(bool debug) {
  _debug = debug;
  _ubiProtocol->setDebug(debug);
}

/*
 * Stores the float type value into the char array input
 * @str_value [Mandatory] char payload pointer to store the value.
 * @value [Mandatory] Float value to convert
 */

void UbiProtocolHandler::_floatToChar(char *str_value, float value) {
  char temp_arr[20];
  sprintf(temp_arr, "%17g", value);
  uint8_t j = 0;
  uint8_t k = 0;
  while (j < 20) {
    if (temp_arr[j] != ' ') {
      str_value[k] = temp_arr[j];
      k++;
    }
    if (temp_arr[j] == '\0') {
      str_value[k] = temp_arr[j];
      break;
    }
    j++;
  }
}

bool UbiProtocolHandler::serverConnected() { return _ubiProtocol->serverConnected(); }
