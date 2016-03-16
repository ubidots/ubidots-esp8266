#include "UbidotsMicroESP8266.h"
/**
 * Constructor.
 */
Ubidots::Ubidots(char* token){
    _token = token;
    maxValues = 3;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id){
  float num;
  String response;
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  if (_client.connect(SERVER, PORT)){
        Serial.println(F("Geting your variable"));
        // Make a HTTP request:
        _client.print(F("GET /api/v1.6/variables/"));
        _client.print(id);
        _client.println(F("/values?page_size=1 HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: Arduino-Ethernet/1.0")); 
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println();
    }
    while (!_client.available());
    while (_client.available()){
        response = _client.readString();
    }
        //Serial.write(c);
    Serial.println(response);
    bodyPosinit =4+ response.indexOf("\r\n\r\n");
    response = response.substring(bodyPosinit);
    Serial.println(response);
    bodyPosinit =9+ response.indexOf("\"value\":");
    bodyPosend = response.indexOf(", \"timestamp\"");
    num = response.substring(bodyPosinit,bodyPosend).toFloat();
    _client.flush();
    _client.stop();
    return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value){
  (val+currentValue)->id = variable_id;
  (val+currentValue)->value_id = value;
  currentValue++;
  if(currentValue>maxValues){
    Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
    currentValue = maxValues;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(){
    int i;
    String all;
    String str;
    all = "[";
    for(i=0; i<currentValue;){
        str = String(((val+i)->value_id),5);
        all += "{\"variable\": \"";
        all += String((val + i)->id);
        all += "\", \"value\":";
        all += str;
        all += "}";
        i++;
        if(i<currentValue){
            all += ", "; 
        }
    }
    all += "]";
    i = all.length();
    
    if (_client.connect(SERVER, PORT)){
          Serial.println(F("Posting your variables"));
          _client.println(F("POST /api/v1.6/collections/values/?force=true HTTP/1.1"));
          _client.println(F("Host: things.ubidots.com"));
          _client.println(F("User-Agent: Arduino-Ethernet/1.0"));
          _client.print(F("X-Auth-Token: "));
          _client.println(_token);
          _client.println(F("Connection: close"));
          _client.println(F("Content-Type: application/json"));
          _client.print(F("Content-Length: "));
          _client.println(String(i));
          _client.println();
          _client.println(all);
          _client.println();          
    }
    while(!_client.available());
    while (_client.available()){
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    _client.stop();
    return true;    
}
bool Ubidots::wifiConnection(char* ssid, char* pass){
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}