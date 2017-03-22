
# ESP8266 nodeMCU 1.0

The Development Kit based on ESP8266, integates GPIO, PWM, IIC, 1-Wire and ADC all in one board. Power your developement in the fastest way combinating with NodeMCU Firmware!

## Requiremets

* [Node MCU version 1.0](http://nodemcu.com/)
* [Arduino IDE 1.6.5 or higher](https://www.arduino.cc/en/Main/Software)
* [UbidotsMicroESP8266 library](https://github.com/ubidots/ubidots-nodemcu/archive/master.zip)

## Setup

1. Go to the Arduino IDE, click on **Files -> Preferences** and enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas
2. Open Boards Manager from **Tools -> Board menu** and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation)
3. Download the UbidotsMicroESP8266 library [here](https://github.com/ubidots/ubidots-nodemcu/archive/master.zip)
4. Now, click on **Sketch -> Include Library -> Add .ZIP Library**
5. Select the .ZIP file of UbidotsMicroESP8266 and then "Accept" or "Choose"
6. Close the Arduino IDE and open it again.

Note: The library will create a new Ubidots data source named "ESP8266", there the library will save the variables that you will send.
    
## Send multiple values to Ubidots 

To send multiple values to Ubidots, go to **Sketch -> Examples ->  ubidots-nodemcu-master library** and select the "UbidotsSaveMultiValues" example. Add your Ubidots TOKEN where indicated, as well as the WI-FI settings.

With code below you'll be able to send data to Ubidots using the variables' label:  

```c++
#include "UbidotsMicroESP8266.h"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    float value = analogRead(A0);
    float value2 = digitalRead(D1);
    float value3 = digitalRead(D2);
    client.add("Temperature", value);
    client.add("Humidity", value2);
    client.add("Heat Index", value3);
    client.sendAll(true);
    delay(10000);
}

```

With code below you'll be able to send data to Ubidots using the variables' ID: 

```c++
#include "UbidotsMicroESP8266.h"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define ID_1 "Your_variable_ID_here" // Put your variable ID 
#define ID_2 "Your_variable_ID_here" // Put your variable ID 
#define ID_3 "Your_variable_ID_here" // Put your variable ID 
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
}

void loop(){
    float value = analogRead(A0);
    float value2 = digitalRead(D1);
    float value3 = digitalRead(D2);
    client.add(ID_1, value1);
    client.add(ID_2, value2);
    client.add(ID_3, value3);
    client.sendAll(false);
    delay(10000);
    }
```
## Get one value from Ubidots

To get the last value of a variable from Ubidots, go to **Sketch -> Examples ->  ubidots-nodemcu-master library** and select the "UbidotsGetValue" example. Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI settings.

With code below you'll be able to get data from Ubidots using the variable ID 

```c++
#include "UbidotsMicroESP8266.h"

#define ID  "Your_VariableID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop() {
    float value = client.getValue(ID);
    Serial.print("Value: ");
    Serial.println(value);
    delay(10000); 
}
```

With code below you'll be able to get data from Ubidots using the variable Label: 
```c++
#include "UbidotsMicroESP8266.h"

#define DEVICE_LABEL "..." // Put here your Device Label
#define VARIABLE_LABEL "..." // Put here your Variable Label
#define TOKEN  "..."  // Put here your Ubidots TOKEN
#define WIFISSID "..." // Your SSID
#define PASSWORD "..." // Your Wi-Fi password

Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  delay(10);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {

  float value = client.getValueWithDevice(DEVICE_LABEL, VARIABLE_LABEL);
  Serial.print("Value: ");
  Serial.println(value);
  delay(10000);
}
```

## Get variable's timestamp
To get the variable's timestamp, go to **Sketch -> Examples ->  ubidots-nodemcu-master library** and select the "UbidotsGetVarTimestamp" example. Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI settings

```c++
#include "UbidotsMicroESP8266.h"

#define ID  "Your_VariableID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop() {
    long timestamp = client.getVarTimestamp(ID); 
    Serial.print("Timestamp: ");
    Serial.println(timestamp);
    delay(10000);
}

```

## Get variable's context
To get the variable's context, go to **Sketch -> Examples ->  ubidots-nodemcu-master library** and select the "UbidotsGetVarContext" example. Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI settings

```c++
#include "UbidotsMicroESP8266.h"

#define ID  "Your_VariableID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop() {
    char* context = client.getVarContext(ID);
    Serial.print("Context: ");
    Serial.println(context);
    delay(10000);
}

```
