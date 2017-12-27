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
Modified by: Maria Carlina Hernandez for Ubidots

*/

#include "UbidotsMicroESP8266.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Constructor.
 */
Ubidots::Ubidots(char* token, char* server) {

  _token = token;
  _server = server;
  _dsName = DEFAULT_DEVICE_NAME;
  maxValues = 5;
  currentValue = 0;
  val = (Value *)malloc(maxValues*sizeof(Value));
  idAsMac();
}

void Ubidots::idAsMac(){
  unsigned char mac[6];
  WiFi.macAddress(mac);
  for (int i = 0; i < 6; i++){
    if ((int)mac[i] < 10){
      sprintf(_espID, "%s0%X", _espID, mac[i]);
    } else {
      sprintf(_espID, "%s%2X", _espID, mac[i]);
    }
  }
}



void Ubidots::setDataSourceName(char *dataSourceName) {

  _dsName = dataSourceName;
}
void Ubidots::setDataSourceLabel(char* dataSourceLabel) {

  _espID = dataSourceLabel;
}


/**
 * This function is to get variable timestamp from the Ubidots API
 * @arg id is the ID of the variable where you will get the timestamp
 * @return VarTimestamp is the timestamp of the variable that you get from the Ubidots API
 */
long Ubidots::getVarTimestamp(char* id) {

  char* data = (char *) malloc(sizeof(char) * 700);
  char* response = (char *) malloc(sizeof(char) * 400);
  char* final = (char *) malloc(sizeof(char) * 16);
  sprintf(data, "GET /api/v1.6/variables/%s", id);
  sprintf(data, "%s/values?page_size=1 HTTP/1.1\r\n", data);
  sprintf(data, "%sHost: things.ubidots.com\r\nUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
  sprintf(data, "%sX-Auth-Token: %s\r\nConnection: close\r\n\r\n", data, _token);

  if (_client.connect(HTTPSERVER, HTTPPORT)) {
    if (_debug){
      Serial.println(F("Getting your variable timestamp: "));
    }
    _client.println(data);
  } else {
    ERROR_VALUE;
  }

  int timeout = 0;
  free(data);
  while(!_client.available() && timeout <= 5000) {
    timeout++;
    if (timeout == 4999){
      return ERROR_VALUE;
    }
    if (_client.available()){
      break;
    }
    delay(1);
  }

  int i = 0;

  while (_client.available()) {
    response[i++] = (char)_client.read();
    if (i >= 399){
      break;
    }
  }

  if (_debug){
    Serial.println(response);
  }

  _client.flush();
  _client.stop();

  // parses the answer
  char *pch = strchr(response, '[');
  if (pch != NULL){
    char *pch2 = strchr(pch, ':');
    if (pch2 != NULL){
      int index = (int)(pch2 - pch - 1);
      memcpy(final, pch2, index);
      final[0] = '0';  //Replaces ':' by zero
      final[1] = '0';  //Replaces ' ' by zero
      delay(1000);
      long result = atol(final);
      free(response);
      free(final);
      return result;
    }
  }

  free(response);
  free(final);
  return ERROR_VALUE;
}

/**
 * This function is to get variable context from the Ubidots API
 * @arg id is the ID of the variable where you will get the context
 * @return VarContext is the context of the variable that you get from the Ubidots API
 */
char* Ubidots::getVarContext(char* id) {

  char* data = (char *) malloc(sizeof(char) * 700);
  char* response = (char *) malloc(sizeof(char) * 400);

  sprintf(data, "GET /api/v1.6/variables/%s", id);
  sprintf(data, "%s/values?page_size=1 HTTP/1.1\r\n", data);
  sprintf(data, "%sHost: things.ubidots.com\r\nUser-Agent:%s/%s\r\n", data, USER_AGENT, VERSION);
  sprintf(data, "%sX-Auth-Token: %s\r\nConnection: close\r\n\r\n", data, _token);

  if (_client.connect(HTTPSERVER, HTTPPORT)) {
    if (_debug){
      Serial.println(F("Getting your variable context: "));
    }
    _client.println(data);
  } else {
    return "e";
  }

  free(data);
  int timeout = 0;

  while(!_client.available() && timeout < 5000) {
    timeout++;
    if (timeout >= 4999){
        return "e";
    }
    delay(1);
  }

  int i = 0;
  while (_client.available()) {
    response[i++] = (char)_client.read();
    if (i >= 399){
      break;
    }
  }

  if (_debug){
    Serial.println(response);
  }

  if (_debug){
    Serial.println(response);
  }

  // Parses the answer
  char * pch = strchr(response, '[');
  if (pch != NULL){
    char * pch2 = strchr(pch + 2, '{');
    pch = strchr(pch2, '}');
    int index = (int)(pch - pch2 + 1);
    memcpy(_context, pch2, index);
    free(response);
    _context[100] = '\0';
    _client.flush();
    _client.stop();
    return _context;
  }

  free(response);
  return "e";
}

/**
 * This function is to get value using TCP
 * @arg id the id where you will get the data
 * @return num the the last value of the variable from the Ubidots API
 */

float Ubidots::getValue(char* id){

  char* response = (char *) malloc(sizeof(char) * 40);
  char* data = (char *) malloc(sizeof(char) * 700);
  sprintf(data, "%s/%s|GET|%s|%s", USER_AGENT, VERSION, _token, id);
  sprintf(data, "%s|end", data);

  if (_debug){
    Serial.println(data);
  }

  if (_client.connect(SERVER, PORT)) {
    if (_debug){
      Serial.println(F("Getting your variable: "));
    }
    _client.print(data);
  }

  int timeout = 0;
  free(data);

  while(!_client.available() && timeout < 50000) {
    timeout++;
    if (timeout >= 49999){
        Serial.println("timeout");
        return ERROR_VALUE;
    }
    delay(1);
  }

  int i = 0;
  for (int i = 0; i <= 40; i++){
    response[i] = '\0';
  }

   while (_client.available()) {
    response[i++] = (char)_client.read();
  }

  // Parses the answer, Expected "OK|{value}"
  char * pch = strchr(response, '|');
  if (pch != NULL){
    float num;
    pch[0] = '0';
    num = atof(pch);
    free(response);
    _client.stop();
    return num;
  }

  free(response);
  return ERROR_VALUE;
}

/**
 * This function is to get value from the Ubidots API using the device and variable label
 * @arg dsLabel is the label of Device
 * @arg varLabel is the label of the variable
 * @return num the the last value of the variable from the Ubidots API
 */

float Ubidots::getValueWithDevice(char* dsLabel, char* varLabel){

  char* data = (char *) malloc(sizeof(char) * 700);
  char* response = (char *) malloc(sizeof(char) * 40);
  sprintf(data, "%s/%s|LV|%s|%s:%s", USER_AGENT, VERSION, _token, dsLabel, varLabel);
  sprintf(data, "%s|end", data);

  if (_debug){
    Serial.println(data);
  }

  if (_client.connect(SERVER, PORT)) {
    if (_debug){
      Serial.println(F("Getting your variable: "));
    }
    _client.print(data);
  }

  int timeout = 0;
  free(data);

  while(!_client.available() && timeout < 50000) {
    timeout++;
    if (timeout >= 49999){
        Serial.println("timeout");
        return ERROR_VALUE;
    }
    delay(1);
  }

  int i = 0;
  for (int i = 0; i <= 40; i++){
    response[i] = '\0';
  }

   while (_client.available()) {
    response[i++] = (char)_client.read();
  }

  // Parses the answer, Expected "OK|{value}"
  char * pch = strchr(response, '|');
  if (pch != NULL){
    float num;
    pch[0] = '0';
    num = atof(pch);
    free(response);
    _client.stop();
    return num;
  }

  free(response);
  return ERROR_VALUE;
}


/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */

void Ubidots::add(char *variable_id, float value) {
  return add(variable_id, value, NULL, NULL);
}
void Ubidots::add(char *variable_id, float value, char *ctext) {
  return add(variable_id, value, ctext, NULL);
}
void Ubidots::add(char *variable_id, float value, unsigned long timestamp) {
  return add(variable_id, value, NULL, timestamp);
}
void Ubidots::add(char *variable_id, float value, char *ctext, unsigned long timestamp) {
  (val+currentValue)->id = variable_id;
  (val+currentValue)->value_id = value;
  (val+currentValue)->context = ctext;
  (val+currentValue)->timestamp = timestamp;
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
  char* data = (char *) malloc(sizeof(char) * 700);

  sprintf(data, "%s/%s|POST|%s|%s:%s=>", USER_AGENT, VERSION, _token, _espID, _dsName);

  for (i = 0; i < currentValue;) {
     str = String(((val+i)->value_id), 5);

    sprintf(data, "%s%s:%s", data, (val + i)->id, str.c_str());

    if ((val + i)->timestamp != NULL) {
      sprintf(data, "%s@%lu%s", data, (val + i)->timestamp, "000");
    }
    if ((val + i)->context != NULL) {
      sprintf(data, "%s$%s", data, (val + i)->context);
    }

    i++;

    if (i < currentValue) {
      sprintf(data, "%s,", data);
    } else {
      sprintf(data, "%s|end", data);
      currentValue = 0;
    }
  }

  Serial.println("");

  if (_debug){
   Serial.println(data);
  }

  if (_client.connect(SERVER, PORT)) {
    _client.print(data);
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
  free(data);
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

  String toPost = "POST /api/v1.6/collections/values/?force=true HTTP/1.1\r\n"
          "Host: things.ubidots.com\r\n"
          "User-Agent:" + String(USER_AGENT) + "/" + String(VERSION) + "\r\n"
          "X-Auth-Token: " + String(_token) + "\r\n"
          "Connection: close\r\n"
          "Content-Type: application/json\r\n"
          "Content-Length: " + String(i) + "\r\n"
          "\r\n"
          + all +
          "\r\n";

  if (_client.connect(HTTPSERVER, HTTPPORT)) {
    Serial.println(F("Posting your variables: "));
    Serial.println(toPost);
    _client.print(toPost);
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

void Ubidots::setDebug(bool debug){
   _debug = debug;
}


bool Ubidots::wifiConnection(char* ssid, char* pass) {
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

/*
 * © Francesco Potortì 2013 - GPLv3 - Revision: 1.13
 *
 * Send an NTP packet and wait for the response, return the Unix time
 *
 * To lower the memory footprint, no buffers are allocated for sending
 * and receiving the NTP packets.  Four bytes of memory are allocated
 * for transmision, the rest is random garbage collected from the data
 * memory segment, and the received packet is read one byte at a time.
 * The Unix time is returned, that is, seconds from 1970-01-01T00:00.
 */

unsigned long Ubidots::ntpUnixTime () {
  static int udpInited = udp.begin(123); // open socket on arbitrary port

  // Only the first four bytes of an outgoing NTP packet need to be set
  // appropriately, the rest can be whatever.
  const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

  // Fail if WiFiUdp.begin() could not init a socket
  if (! udpInited)
    return 0;

  // Clear received data from possible stray received packets
  udp.flush();

  // Send an NTP request
  if (! (udp.beginPacket(TIME_SERVER, 123) // 123 is the NTP port
    && udp.write((byte *)&ntpFirstFourBytes, 48) == 48
    && udp.endPacket()))
      return 0;         // sending request failed

  // Wait for response; check every pollIntv ms up to maxPoll times
  const int pollIntv = 150;   // poll every this many ms
  const byte maxPoll = 15;    // poll up to this many times
  int pktLen;         // received packet length
  for (byte i = 0; i < maxPoll; i++) {
    if ((pktLen = udp.parsePacket()) == 48)
      break;
    delay(pollIntv);
  }
  if (pktLen != 48)
    return 0;         // no correct packet received

  // Read and discard the first useless bytes
  // Set useless to 32 for speed; set to 40 for accuracy.
  const byte useless = 40;
  for (byte i = 0; i < useless; ++i)
    udp.read();

  // Read the integer part of sending time
  unsigned long time = udp.read();  // NTP time
  for (byte i = 1; i < 4; i++)
    time = time << 8 | udp.read();

  // Round to the nearest second if we want accuracy
  // The fractionary part is the next byte divided by 256: if it is
  // greater than 500ms we round to the next second; we also account
  // for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
  // additionally, we account for how much we delayed reading the packet
  // since its arrival, which we assume on average to be pollIntv/2.
  time += (udp.read() > 115 - pollIntv/8);
  // Discard the rest of the packet
  udp.flush();
  return time - 2208988800ul;     // convert NTP time to Unix time
}

/*******************************************
Deprecated functions
*******************************************/

float Ubidots::getValueUDP(char *id){
  Serial.println("This function is Deprecated, please use the getValue() method");
  return getValue(id);
}