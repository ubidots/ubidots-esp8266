/*
 * © Oscar Neira 2017 - GPLv3 - Revision: 1.13
 * Extension of Ubidots client to support HTTPS connections with
 * verification of server certificate
 */

#include "UbidotsMicroESP8266.h"
#include "UbidotsRootCA.h"
#include <time.h>

#define HTTPSPORT 443

class UbidotsSecure : public Ubidots {
  public:
    UbidotsSecure(char* token, char* server = SERVER);
    virtual bool _connect(char * host, int port);
  protected:
	WiFiClientSecure _client;
  private:
  	bool SNTPinitialized;
	void initSNTP();
};


UbidotsSecure::UbidotsSecure(char* token, char* server): Ubidots(token, server) {
	SNTPinitialized = false;
	// Load root certificate in DER format into WiFiClientSecure object
	bool success;
	success = _client.setCACert(caCert, caCertLen);
	if (_debug){
	  Serial.printf("setCACert: %d\n", (int) success);
	}
}

/**
 * This function initializes SNTP to check certificate expiry dates
 * To be called just after wifi connection is stablished or before
 * checking certificate validity
 */
void UbidotsSecure::initSNTP(){
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
	Serial.print(".");
	delay(1000);
  }
  SNTPinitialized = true;
}


/**
 * This function connects to host and verifies secure connection
 * using a stored fingerprint. If fingerprint fails, certificate
 * chain is validated against a stored root certificate
 * @arg host the host to connect
 * @arg port the port to connect (overrided to HTTPSPORT)
 */
bool UbidotsSecure::_connect(char * host, int port){
  if (!_client.connect(host, HTTPSPORT)) {
      return false;
    }
  if (_client.verify(UBIDOTS_FINGERPRINT, host)) {
    if (_debug){
      Serial.println("Server certificate matches stored fingerprint");
    }
    return true;
  } else {
    if (_debug){
      Serial.println("Server certificate doesn't match stored fingerprint");
    }
  }
  // If fingerprint verification fails, verify validity of server's certificate
  // As dates are checked, sntp data has to be initialized by calling
  // initSNTP()
  if (!SNTPinitialized){
	  initSNTP();
  }
  if (_client.verifyCertChain(host)) {
    if (_debug){
      Serial.println("Server certificate verified");
    }
    return true;
  } else {
    if (_debug){
      Serial.println("ERROR: certificate chain verification failed!");
    }
    return false;
  }
}

