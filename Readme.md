# ESP8266-1 stand-alone

The ESP8266 is a low-cost WiFi module built by Espressif Systems. Its popularity has been growing among the hardware community thanks to it’s nice features and stability, to the point that it can be easily programmed using your Arduino IDE.

## Requiremets

* [An ESP8266 module](https://www.sparkfun.com/products/13678).
* [An Arduino UNO](https://www.arduino.cc/en/Main/ArduinoBoardUno), [UartSBee](http://www.seeedstudio.com/wiki/UartSBee_V4) or any Uart to USB device.
* [Arduino IDE 1.6.5 or higher](https://www.arduino.cc/en/Main/Software)
* [UbidotsMicroESP8266 library](https://github.com/ubidots/ubidots-esp8266/archive/master.zip) 
* Jumper wires.

**NOTE:** This library is also compatible with the **NodeMcu v1.0**, on our [help center](http://help.ubidots.com/connect-your-devices) you'll find a guide with the setup and examples codes.   
  
## Wires

First we’ll need to identify the pinout of ESP8266.

![ESP8266 connection](https://raw.githubusercontent.com/guyz/pyesp8266/master/esp8266_pinout.png)

ESP8266 | UARTbee | Arduino UNO
--------|---------|------------ 
RXD | TXD | RX
GPIO0 | GND | GND
GPIO2 | — | —
GND | GND | GND
VCC | 3.3V | 3.3V
RST | — | —
CH_PD | 3.3V | 3.3V
TXD | RXD | TX

**Note:** If you are using an Arduino UNO you will need to set Arduino RST to GND. Please be careful with the VCC of the ESP8266, it works only with a 3.3V supply.

## Setup

1. Download [Arduino IDE](https://www.arduino.cc/en/Main/Software).
2. Open you IDE and click on **File -> Preferences**.
3. In Aditional **Boards Manager URLs** add this line and click on “OK”: *“http://arduino.esp8266.com/stable/package_esp8266com_index.json”*
4. Open Boards Manager from **Tools -> Board menu** and install esp8266 platform.
5. Select the ESP8266 board from **Tools > Board > Generic ESP8266 Module**.
6. Download the **UbidotsMicroESP8266 library** [here](https://github.com/ubidots/ubidots-nodemcu/archive/master.zip).
7. Now, click on **Sketch -> Include Library -> Add .ZIP Library**.
8. Select the .ZIP file of UbidotsMicroESP8266 and then "**Accept**" or "**Choose**".
9. Close the Arduino IDE and open it again.

**Note**: The library will create a new Ubidots data source named "**ESP8266**" getting the mac of the ESP8266 as device label. If you desire assign a different device name and label, you can add the add to your code the following lines

# Documentation

## Constructor

### Ubidots

```
Ubidots(char* token)
Ubidots(char* token, const char * server)
```
> Creates an Ubidots instance, you must setup as input your Ubidots TOKEN, the server is optional and is set by default to "industrial.api.ubidots.com" for sending data using an Industrial account. You can set another endpoint using the `setServerEndpoint()` method.

## Methods

### Ubidots

```
add(char *variable_label, float value)
add(char *variable_label, float value, char *ctext)
add(char *variable_label, float value, unsigned long timestamp)
add(char *variable_label, float value, char *ctext, unsigned long timestamp)
```
> Adds a dot with a value, context or timestamp to be sent to a certain device, once you use add() you can send data your variable using the `sendAll()` method. You can add up to 5 dots in the buffer to send. Context and timestamp values are optional.

```
getValueWithDevice(char* deviceLabel, char* varLabel)
```

> Returns as float the last value of a variable using both device and variable labels. For more information, please refere (here)[https://help.ubidots.com/faqs-and-troubleshooting/where-can-i-find-the-variable-id]. If an error is found during the data retrievel, it will return the ERROR_VALUE constant, that is equal to -3.4028235E+8.

```
getValue(char *variableId)
```

> Returns as float the last value of a variable using its (id)[https://help.ubidots.com/faqs-and-troubleshooting/where-can-i-find-the-variable-id]. If an error is found during the data retrievel, it will return the ERROR_VALUE constant, that is equal to -3.4028235E+8.

```
getVarTimestamp(char* variableId)
```

> Returns as float the last value timestamp of a variable using its (id)[https://help.ubidots.com/faqs-and-troubleshooting/where-can-i-find-the-variable-id]. If an error is found during the data retrievel, it will return the ERROR_VALUE constant, that is equal to -3.4028235E+8.

```
getVarContext(char* variableId)
```

> Returns as float the last value context stored of a variable using its (id)[https://help.ubidots.com/faqs-and-troubleshooting/where-can-i-find-the-variable-id]. If an error is found during the data retrievel, it will return the 'e' char

```
setServerEndpoint(const char * server)
```
> Sets the endpoint url properly to send or retrieve data from Ubidots. If your account if a business one, set "industrial.api.ubidots.com" or the endpoint provided by Ubidots as your endpoint url, see examples for more information.
If you are using a free educational version accounts, set the endpoint to "things.ubidots.com" for HTTP or "translate.ubidots.com" for TCP.

```
setDeviceType(const char * deviceType)
```

> Sends data through HTTP using the device type label from the method argument. For more information about device types, please refer (here)[https://help.ubidots.com/user-guides/device-types]

```
sendAll(bool type = false)
```

> Sends data to Ubidots. If type is set to false, TCP will be used, if not, data will be sent using HTTP. Returns true if data is sent sucessfully.

```
setDebug(bool debug)
```

> Make available debug messages through the serial port.

```
wifiConnection(char* ssid, char* pass)
```
> Connect via Wifi to the specified SSID.

#Examples


Please refer to the Examples path to reference different apps.




