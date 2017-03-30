
# ESP8266 nodeMCU 1.0

The Development Kit based on ESP8266, integates GPIO, PWM, IIC, 1-Wire and ADC all in one board. Power your developement in the fastest way combinating with NodeMCU Firmware!

## Requiremets

* [Node MCU version 1.0](http://nodemcu.com/)
* [Arduino IDE 1.6.5 or higher](https://www.arduino.cc/en/Main/Software)
* [UbidotsMicroESP8266 library](https://github.com/ubidots/ubidots-nodemcu/archive/master.zip)

## Setup

1. Go to the Arduino IDE, click on **Files -> Preferences** and enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
2. Open Boards Manager from **Tools -> Board menu** and install esp8266 platform.
3. Select the ESP8266 board from **Tools > Board > Generic ESP8266 Module**.
4.  Download the UbidotsMicroESP8266 library [here](https://github.com/ubidots/ubidots-nodemcu/archive/master.zip).
5.  Now, click on **Sketch -> Include Library -> Add .ZIP Library**.
6.  Select the .ZIP file of UbidotsMicroESP8266 and then "**Accept**" or "**Choose**".
7.  Close the Arduino IDE and open it again.

Note: The library will create a new Ubidots data source named "**ESP8266**", there the library will save the variables that you will send.
    
## Send values to Ubidots 

You send values using the variable label or the variable ID, also you can send values with timestamp and context. 

### Send values using the variable label 

The following example is to send one value to Ubidots, it will create the variable automatically with the label assign by you on the code. You can send until 5 differentes values to Ubidots, if you desire send more values just add this line ```client.add("variable_name", value);``` to your code with the parameters needed by you. 

Also, you can find the example on the library examples. Go to **Sketch -> Examples -> ubidots-nodemcu-master library** and select the **"UbidotsSaveValueWithLabel"** example.

Add your Ubidots TOKEN where indicated, as well as the WI-FI settings.

```c++
#include "UbidotsMicroESP8266.h"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    float value1 = analogRead(A0);
    //float value2 = digitalRead(D1)
    client.add("temperature", value1);
    //client.add("switch", value2);
    client.sendAll(true);
}
```

### Send values using the variable ID

The following example is to send one value to Ubidots, you have to assing the variable ID to the code. Take a look of this [guide](http://help.ubidots.com/ubidots-app/how-to-get-your-variable-id-into-your-ubidots-account) to know where you can get the variable ID. You can send until 5 differentes values to Ubidots, if you desire send more values just add this line ```client.add(ID, value);``` to your code with the parameters needed by you. 

Also, you can find the example on the examples folder. Go to **Sketch -> Examples -> ubidots-nodemcu-master library** and select the **"UbidotsSaveValueWithID"** example.

Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI settings.

```c++
#include "UbidotsMicroESP8266.h"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define ID_1 "Your_variable_ID_here" // Put your variable ID here
//#define ID_2 "Your_variable_ID_here" // Put your variable ID here
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    float value1 = analogRead(A0);
    //float value2 = digitalRead(D1)
    client.add(ID_1, value1);
    //client.add(ID_2, value2);
    client.sendAll(false);
}
```

### Send values with timestamp

The following example is to send one value with timestamp to Ubidots, it will create the variable automatically with the label assign by you on the code.

Add your Ubidots TOKEN where indicated, as well as the WI-FI settings.

```c++
#include "UbidotsMicroESP8266.h"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    float value = analogRead(A0);
    unsigned long t = client.ntpUnixTime(); // calculates your actual timestamp in SECONDS

    client.add("humidity", value, NULL, t-20000);
    client.sendAll(true);  
    delay(5000);
}
```


### Send values with context

The following example is to send one value with context to Ubidots, it will create the variable automatically with the label assign by you on the code.

Add your Ubidots TOKEN where indicated, as well as the WI-FI settings.

```c++
#include "UbidotsMicroESP8266.h"

#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    float value = analogRead(A0);
    char context[25];
    sprintf(context, "lat=1.2343$lng=132.1233"); //Sends latitude and longitude for watching position in a map

    client.add("humidity", value, context);
    client.sendAll(true); 
    delay(5000)
}
```


## Get value from Ubidots

To get the last value of a variable from Ubidots you can use three different functions:

+ client.getValue(ID)
+ client.getValueUDP(ID)
+ client.getValueWithDevice(DEVICE_LABEL, VARIABLE_LABEL)

On the example folder you'll find all the examples codes. Go to **Sketch -> Examples ->  ubidots-nodemcu-master library** and select the one that you need "**UbidotsGetValue**";"**UbidotsGetValueUDP**";"**UbidotsGetValueWithDevice**".

### client.getValue(ID)

This function let you get de last value of a variable using the variable ID.

Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI settings.

```c++
#include "UbidotsMicroESP8266.h"

#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
    float value = client.getValue(ID);
    Serial.print("Value: ");
    Serial.println(value);
    delay(10000);
}
```

### client.getValueUDP(ID);

This function let you get de last value of a variable using the variable ID.

Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI 
settings.

```c++
#include "UbidotsMicroESP8266.h"

#define ID  "..."  // Put here your Ubidots variable ID
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

  float value = client.getValueUDP(ID);
  Serial.print("Value: ");
  Serial.println(value);
  delay(10000);
}
```

### client.getValueWithDevice(DEVICE_LABEL, VARIABLE_LABEL)

This function let you get de last value of a variable using the device and varriable labels.

Add your Ubidots TOKEN, device label, and variable label where indicated, as well as the WI-FI settings.

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

This function let you get the variable's timestamp using the variable ID.

On the example folder you'll find the example code. Go to **Sketch -> Examples ->  ubidots-nodemcu-master library** and select the "**UbidotsGetVarTimestamp**" example. 

Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI settings

```c++
#include "UbidotsMicroESP8266.h"

#define ID  "Your_VariableID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop() {
    long timestamp = client.getVarTimestamp(ID);
    Serial.print("timestamp: ");
    Serial.println(timestamp);
    delay(10000);
}
```

## Get variable's context

This function let you get the variable's context using the variable ID.

On the example folder you'll find the example code. Go to **Sketch -> Examples ->  ubidots-nodemcu-master library** and select the "**UbidotsGetVarContext**" example. 

Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI settings

```c++
#include "UbidotsMicroESP8266.h"

#define ID  "Your_VariableID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID" // Put here your Wi-Fi SSID
#define PASSWORD "Your_WiFi_Password" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop() {
    char* context = client.getVarContext(ID);
    Serial.print("context: ");
    Serial.println(context);    
    delay(10000);
}
```






This library is also compatible with the **ESP8266-1 stand alone**, on our [help center](http://help.ubidots.com/connect-your-devices) you'll find a guide with the setup and examples codes.   