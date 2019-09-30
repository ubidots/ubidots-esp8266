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

#ifndef _UbiConstants_H_
#define _UbiConstants_H_

#include "UbiTypes.h"

const char* const UBIDOTS_SERVER = "industrial.api.ubidots.com";
const char* const USER_AGENT = "UbidotsESP8266/3.0.0";
const char* const UBIDOTS_MESH_CHANNEL = "ubidots-mesh";
const int UBIDOTS_HTTP_PORT = 80;
const int UBIDOTS_TCP_PORT = 9012;
const uint8_t MAX_VALUES = 10;
const float ERROR_VALUE = -3.4028235E+8;
const int MAX_BUFFER_SIZE = 700;
static UbiServer UBI_INDUSTRIAL = "industrial.api.ubidots.com";

#endif
