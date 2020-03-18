/*
Copyright (c) 2013-2020 Ubidots.
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

#include "UbiTcp.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

UbiTCP::UbiTCP(const char *host, const int port, const char *user_agent, const char *token) {
  _host = host;
  _user_agent = user_agent;
  _token = token;
  _port = port;
  _syncronizeTime();
  _certifiedLoaded = _loadCert();
}

/**************************************************************************
 * Destructor
 ***************************************************************************/

UbiTCP::~UbiTCP() {
  delete[] _host;
  delete[] _user_agent;
  delete[] _token;
}

/**************************************************************************
 * Cloud Functions
 ***************************************************************************/

bool UbiTCP::sendData(const char *device_label, const char *device_name, char *payload) {
  bool allowed = _preConnectionChecks();
  if (!allowed) {
    return false;
  }

  /* Connecting the client */
  _client_tcps_ubi.connect(_host, UBIDOTS_TCPS_PORT);
  reconnect(_host, UBIDOTS_TCPS_PORT);
  if (!_client_tcps_ubi.verifyCertChain(_host)) {
    if (_debug) {
      Serial.println("[ERROR] Could not verify the remote secure server certificate, "
                     "please make sure that you are using a secure "
                     "network");
    }
    return false;
  }

  /* Sends data to Ubidots */
  if (_client_tcps_ubi.connected()) {
    _client_tcps_ubi.print(payload);
  } else {
    if (_debug) {
      Serial.println("[ERROR] Could not connect to the host");
    }
    _client_tcps_ubi.stop();
    return false;
  }

  /* Waits for the host's answer */
  if (!waitServerAnswer()) {
    if (_debug) {
      Serial.println("[ERROR] Could not read server's response");
    }
    _client_tcps_ubi.stop();
    return false;
  }

  /* Parses the host answer, returns true if it is 'Ok' */
  char *response = (char *)malloc(sizeof(char) * 100);

  float value = parseTCPAnswer("POST", response);
  free(response);
  if (value != ERROR_VALUE) {
    _client_tcps_ubi.stop();
    return true;
  }

  _client_tcps_ubi.stop();
  return false;
}

double UbiTCP::get(const char *device_label, const char *variable_label) {
  bool allowed = _preConnectionChecks();
  if (!allowed) {
    return ERROR_VALUE;
  }

  /* Connecting the client */
  _client_tcps_ubi.connect(_host, UBIDOTS_TCPS_PORT);
  reconnect(_host, UBIDOTS_TCPS_PORT);

  if (!_client_tcps_ubi.verifyCertChain(_host)) {
    if (_debug) {
      Serial.println("[ERROR] Could not verify the remote secure server certificate, "
                     "please make sure that you are using a secure "
                     "network");
    }
    return ERROR_VALUE;
  }

  if (_client_tcps_ubi.connected()) {
    /* Builds the request POST - Please reference this link to know all the
     * request's structures https://ubidots.com/docs/api/ */
    _client_tcps_ubi.print(_user_agent);
    _client_tcps_ubi.print("|LV|");
    _client_tcps_ubi.print(_token);
    _client_tcps_ubi.print("|");
    _client_tcps_ubi.print(device_label);
    _client_tcps_ubi.print(":");
    _client_tcps_ubi.print(variable_label);
    _client_tcps_ubi.print("|end");

    if (_debug) {
      Serial.println("----");
      Serial.println("Payload for request:");
      Serial.print(_user_agent);
      Serial.print("|LV|");
      Serial.print(_token);
      Serial.print("|");
      Serial.print(device_label);
      Serial.print(":");
      Serial.print(variable_label);
      Serial.print("|end");
      Serial.println("\n----");
    }

    /* Waits for the host's answer */
    if (!waitServerAnswer()) {
      _client_tcps_ubi.stop();
      return ERROR_VALUE;
    }

    /* Reads the response from the server */
    char *response = (char *)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    float value = parseTCPAnswer("LV", response);
    _client_tcps_ubi.stop();
    free(response);
    return value;
  }

  if (_debug) {
    Serial.println("ERROR could not connect to the server");
  }

  _client_tcps_ubi.stop();
  return ERROR_VALUE;
}

/**************************************************************************
 * Auxiliar
 ***************************************************************************/

/**
 * Reconnects to the server
 * @return true once the host answer buffer length is greater than zero,
 *         false if timeout is reached.
 */

void UbiTCP::reconnect(const char *host, const int port) {
  uint8_t attempts = 0;
  while (!_client_tcps_ubi.status() && attempts < 5) {
    if (_debug) {
      Serial.print("Trying to connect to ");
      Serial.print(host);
      Serial.print(" , attempt number: ");
      Serial.println(attempts);
    }
    _client_tcps_ubi.stop();
    _client_tcps_ubi.connect(host, port);
    attempts += 1;
    delay(1000);
  }
}

/**
 * Function to wait for the host answer up to the already set _timeout.
 * @return true once the host answer buffer length is greater than zero,
 *         false if timeout is reached.
 */

bool UbiTCP::waitServerAnswer() {
  int timeout = 0;
  while (!_client_tcps_ubi.available() && timeout < _timeout) {
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
 * Parse the TCP host answer and saves it to the input char pointer.
 * @payload [Mandatory] char payload pointer to store the host answer.
 * @return true if there is an 'Ok' in the answer, false if not.
 */

float UbiTCP::parseTCPAnswer(const char *request_type, char *response) {
  int j = 0;

  if (_debug) {
    Serial.println("----------");
    Serial.println("Server's response:");
  }

  while (_client_tcps_ubi.available()) {
    char c = _client_tcps_ubi.read();
    if (_debug) {
      Serial.write(c);
    }
    response[j] = c;
    j++;
    if (j >= MAX_BUFFER_SIZE - 1) {
      break;
    }
  }

  if (_debug) {
    Serial.println("\n----------");
  }

  response[j] = '\0';
  float result = ERROR_VALUE;

  // POST
  if (request_type == "POST") {
    char *pch = strstr(response, "OK");
    if (pch != NULL) {
      result = 1;
    }
    return result;
  }

  // LV
  char *pch = strchr(response, '|');
  if (pch != NULL) {
    result = atof(pch + 1);
  }

  return result;
}

/**
 * Makes available debug traces
 */

void UbiTCP::setDebug(bool debug) { _debug = debug; }

/*
 * Checks if the socket is still opened with the Ubidots Server
 */

bool UbiTCP::serverConnected() { return _client_tcps_ubi.connected(); }

/*
 * Syncronizes the internal timer to verify if the cert has expired
 */

bool UbiTCP::_syncronizeTime() {
  // Synchronizes time using SNTP. This is necessary to verify that
  // the TLS certificates offered by the server are currently valid.
  if (_debug) {
    Serial.print("Setting time using SNTP");
  }
  configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  uint8_t attempts = 0;
  while (now < 8 * 3600 * 2 && attempts <= 5) {
    if (_debug) {
      Serial.print(".");
    }
    now = time(nullptr);
    attempts += 1;
    delay(500);
  }

  if (attempts > 5) {
    if (_debug) {
      Serial.println("[ERROR] Could not set time using remote SNTP to verify Cert");
    }
    return false;
  }

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  if (_debug) {
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
  }
  return true;
}

/*
 * Loads the certified from the constants file
 */

bool UbiTCP::_loadCert() {
  // Loads root certificate in DER format into WiFiClientSecure object
  bool res = _client_tcps_ubi.setCACert_P(UBI_CA_CERT_1, UBI_CA_CERT_LEN_1);
  res = res && _client_tcps_ubi.setCACert_P(UBI_CA_CERT_2, UBI_CA_CERT_LEN_2);
  if (!res && _debug) {
    Serial.println("Failed to load root CA certificate!");
  }
  return res;
}

bool UbiTCP::_preConnectionChecks() {
  /* Synchronizes time every 60 minutes */
  bool syncronized = true;
  if (millis() - _timerToSync > 3600000) {
    syncronized = _syncronizeTime();
    _timerToSync = millis();
  }

  if (!syncronized) {
    return false;
  }

  if (!_certifiedLoaded) {
    if (_debug) {
      Serial.println("Please load a valid certificate");
    }
    return false;
  }

  return true;
}
