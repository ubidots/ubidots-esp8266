/*
Copyright (c) 2013-2019 Ubidots.
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

#include "UbiHttp.h"
#include "UbiConstants.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

UbiHTTP::UbiHTTP(const char* host, const int port, const char* user_agent, const char* token) {
  _host = host;
  _user_agent = user_agent;
  _token = token;
  _port = port;
}

/**************************************************************************
 * Destructor
 ***************************************************************************/

UbiHTTP::~UbiHTTP() {
  delete[] _host;
  delete[] _user_agent;
  delete[] _token;
}

bool UbiHTTP::sendData(const char* device_label, const char* device_name, char* payload) {
  reconnect(_host, _port);
  bool result = false;

  if (_client_http_ubi.connected()) {  // Connect to the host
    /* Builds the request POST - Please reference this link to know all the
     * request's structures https://ubidots.com/docs/api/ */
    _client_http_ubi.print(F("POST /api/v1.6/devices/"));
    _client_http_ubi.print(device_label);
    _client_http_ubi.print(F(" HTTP/1.1\r\n"));
    _client_http_ubi.print(F("Host: "));
    _client_http_ubi.print(_host);
    _client_http_ubi.print(F("\r\n"));
    _client_http_ubi.print(F("User-Agent: "));
    _client_http_ubi.print(_user_agent);
    _client_http_ubi.print(F("\r\n"));
    _client_http_ubi.print(F("X-Auth-Token: "));
    _client_http_ubi.print(_token);
    _client_http_ubi.print(F("\r\n"));
    _client_http_ubi.print(F("Connection: close\r\n"));
    _client_http_ubi.print(F("Content-Type: application/json\r\n"));
    int content_length = strlen(payload);
    _client_http_ubi.print(F("Content-Length: "));
    _client_http_ubi.print(content_length);
    _client_http_ubi.print(F("\r\n\r\n"));
    _client_http_ubi.print(payload);
    _client_http_ubi.print(F("\r\n"));
    _client_http_ubi.flush();

    if (_debug) {
      Serial.println(F("Making request to Ubidots:\n"));
      Serial.print("POST /api/v1.6/devices/");
      Serial.print(device_label);
      Serial.print(" HTTP/1.1\r\n");
      Serial.print("Host: ");
      Serial.print(_host);
      Serial.print("\r\n");
      Serial.print("User-Agent: ");
      Serial.print(_user_agent);
      Serial.print("\r\n");
      Serial.print("X-Auth-Token: ");
      Serial.print(_token);
      Serial.print("\r\n");
      Serial.print("Connection: close\r\n");
      Serial.print("Content-Type: application/json\r\n");
      Serial.print("Content-Length: ");
      Serial.print(content_length);
      Serial.print("\r\n\r\n");
      Serial.print(payload);
      Serial.print("\r\n");

      Serial.println("waiting for server answer ...");
    }

    /* Reads the response from the server */
    waitServerAnswer();

    if (_debug) {
      Serial.println("\nUbidots' Server response:\n");
      while (_client_http_ubi.available()) {
        char c = _client_http_ubi.read();
        Serial.print(c);
      }
    }

    result = true;

  } else {
    if (_debug) {
      Serial.println("Could not send data to ubidots using HTTP");
    }
  }

  _client_http_ubi.stop();
  return result;
}

float UbiHTTP::get(const char* device_label, const char* variable_label) {
  _client_http_ubi.connect(_host, UBIDOTS_HTTP_PORT);
  reconnect(_host, UBIDOTS_HTTP_PORT);
  if (_client_http_ubi.connected()) {
    /* Builds the request GET - Please reference this link to know all the
     * request's structures HTTPS://ubidots.com/docs/api/ */
    _client_http_ubi.print(F("GET /api/v1.6/devices/"));
    _client_http_ubi.print(device_label);
    _client_http_ubi.print("/");
    _client_http_ubi.print(variable_label);
    _client_http_ubi.print("/lv");
    _client_http_ubi.print(" HTTP/1.1\r\n");
    _client_http_ubi.print("Host: ");
    _client_http_ubi.print(UBIDOTS_HTTP_PORT);
    _client_http_ubi.print("\r\n");
    _client_http_ubi.print("User-Agent: ");
    _client_http_ubi.print(_user_agent);
    _client_http_ubi.print("\r\n");
    _client_http_ubi.print("X-Auth-Token: ");
    _client_http_ubi.print(_token);
    _client_http_ubi.print("\r\n");
    _client_http_ubi.print("Content-Type: application/json\r\n\r\n");

    if (_debug) {
      Serial.print("GET /api/v1.6/devices/");
      Serial.print(device_label);
      Serial.print("/");
      Serial.print(variable_label);
      Serial.print("/lv");
      Serial.print(" HTTP/1.1\r\n");
      Serial.print("Host: ");
      Serial.print(UBIDOTS_HTTP_PORT);
      Serial.print("\r\n");
      Serial.print("User-Agent: ");
      Serial.print(_user_agent);
      Serial.print("\r\n");
      Serial.print("X-Auth-Token: ");
      Serial.print(_token);
      Serial.print("\r\n");
      Serial.print("Content-Type: application/json\r\n\r\n");
    }

    /* Waits for the host's answer */
    if (!waitServerAnswer()) {
      _client_http_ubi.stop();
      return ERROR_VALUE;
    }

    /* Reads the response from the server */
    char* response = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    readServerAnswer(response);

    /* Parses the answer */
    float value = parseHttpAnswer("LV", response);
    _client_http_ubi.stop();
    free(response);
    return value;
  }

  if (_debug) {
    Serial.println("Connection Failed ubidots - Try Again");
  }

  _client_http_ubi.stop();
  return ERROR_VALUE;
}

/**
 * Reconnects to the server
 * @return true once the host answer buffer length is greater than zero,
 *         false if timeout is reached.
 */

void UbiHTTP::reconnect(const char* host, const int port) {
  uint8_t attempts = 0;
  while (!_client_http_ubi.connected() && attempts < _maxReconnectAttempts) {
    if (_debug) {
      Serial.print("Trying to connect to ");
      Serial.print(host);
      Serial.print(" , attempt number: ");
      Serial.println(attempts);
    }
    _client_http_ubi.connect(host, port);
    if (_debug) {
      Serial.println("Attempt finished");
    }
    attempts += 1;
    delay(1000);
  }
}

float UbiHTTP::parseHttpAnswer(const char* request_type, char* data) {
  float result = ERROR_VALUE;

  // LV
  if (request_type == "LV") {
    char* parsed = (char*)malloc(sizeof(char) * 20);
    char* dst = (char*)malloc(sizeof(char) * 20);
    int len = strlen(data);  // Length of the answer char array from the server

    for (int i = 0; i < len - 2; i++) {
      if ((data[i] == '\r') && (data[i + 1] == '\n') && (data[i + 2] == '\r') && (data[i + 3] == '\n')) {
        strncpy(parsed, data + i + 4, 20);  // Copies the result to the parsed
        parsed[20] = '\0';
        break;
      }
    }

    /* Extracts the the value */

    uint8_t index = 0;

    // Creates pointers to split the value
    char* pch = strchr(parsed, '\n');
    if (pch == NULL) {
      return result;
    }

    char* pch2 = strchr(pch + 1, '\n');

    if (pch2 == NULL) {
      return result;
    }

    index = (int)(pch2 - pch - 1);

    sprintf(dst, "%s", pch);
    dst[strlen(dst) - 1] = '\0';

    float result = atof(dst);

    free(dst);
    free(parsed);
    return result;
  }

  return ERROR_VALUE;
}

/**
 * @arg response [Mandatory] Pointer to store the server's answer
 */

void UbiHTTP::readServerAnswer(char* response) {
  // Fills with zeros
  for (int i = 0; i <= MAX_BUFFER_SIZE; i++) {
    response[i] = '\0';
  }

  if (_debug) {
    Serial.println("----------");
    Serial.println("Server's response:");
  }

  int j = 0;

  while (_client_http_ubi.available()) {
    response[j] = (char)_client_http_ubi.read();
    if (_debug) {
      Serial.write(response[j]);
    }
    j++;
    if (j >= MAX_BUFFER_SIZE - 1) {
      break;
    }
  }

  if (_debug) {
    Serial.println("\n----------");
  }
}

/**
 * Function to wait for the host answer up to the already set _timeout.
 * @return true once the host answer buffer length is greater than zero,
 *         false if timeout is reached.
 */

bool UbiHTTP::waitServerAnswer() {
  int timeout = 0;
  while (!_client_http_ubi.available() && timeout < _timeout) {
    timeout++;
    delay(1);
    if (timeout > _timeout - 1) {
      if (_debug) {
        Serial.println("timeout, could not read any response from the host");
      }
      return false;
    }
  }
  return true;
}

/**
 * Makes available debug traces
 */

void UbiHTTP::setDebug(bool debug) { _debug = debug; }

/*
 * Checks if the socket is still opened with the Ubidots Server
 */

bool UbiHTTP::serverConnected() { return _client_http_ubi.connected(); }
