
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
    
## Send one value to Ubidots

To send a value to Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the "saveValue" example. 
Put your Ubidots TOKEN where indicated, as well as the WiFi settings.

```c++
#include "UbidotsMicroESP8266.h"
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    float value = analogRead(A0);
    client.add("Temperature", value);
    client.sendAll(true);
}
```


## Get one value from Ubidots

To get the last value of a variable from Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the "getValue" example. 
Put your Ubidots TOKEN where indicated, as well as the WiFi settings.

```c++
#include "UbidotsMicroESP8266.h"
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);


void setup() {
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
    float value = client.getValue(ID);
}
```

## Send multiple values to Ubidots 

To send multiple values to Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the "saveValues" example. Put your Ubidots TOKEN where indicated, as well as the WiFi settings.

```c++
#include "UbidotsMicroESP8266.h"
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    float value = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    client.add("Temperature", value);
    client.add("Humidity", value2);
    client.add("Heat Index", value3);
    client.sendAll(true);
}

```