/*
 * This project creates a simple website inferface for a
 * coffee maker. Thie website features an API through which the
 * coffee maker state (on/off) and auto turn-on/off alarms
 * can be set. The project is build uppon the webManager library
 * created by myself, which can be found at:
 * https://github.com/ldaug99/ESPWebManager
 * 
 * The ESPWebManager is build on the ESPAsyncWebServer library by
 * me-no-dev and the SPIFFS library by espressif. The webManager
 * library is an attempt to simplify setting up a website
 * on an ESP32. 
 * 
 * The ESPAsyncWebServer library can be found at:
 * https://github.com/me-no-dev/ESPAsyncWebServer
 * 
 * The SPIFFS and Arduino-ESP32 core library can be found at:
 * https://github.com/espressif/arduino-esp32
 * 
 * The library is distrubuted with the GNU Lesser General 
 * Public License v2.1, as per requirement of the Arduino-ESP32
 * and ESPAsyncWebServer library.
 * This library was created by ldaug99.
*/ 

// To upload website files located in the data folder, use:
// platformio run --target uploadfs

// Include needed libraries
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include "ESPWebManager.h"

// Define WiFi SSID and password
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Number of URLs that the user can access
#define contentEntries 5 

// Define all avaliable web pages and resouce elements avaliable to the webserver
const webContentEntry webContent[contentEntries] = {
	{"/", 				"/index.html", 		HTMLfile, 	HTTP_GET},
	{"/myScript.js", 	"/myScript.js", 	RESfile, 	HTTP_GET},
	{"/styles.css", 	"/styles.css", 		RESfile, 	HTTP_GET},
	{"/settings", 		"/settings.html", 	HTMLfile, 	HTTP_GET},
	{"/api", 			"", 				API, 		HTTP_GET | HTTP_POST | HTTP_PUT}
};

// Number of API keywords
#define keywords 2

// API keywords
apiKeyword apiKeywords[] = {
	{"coffeeState", "COFFEESTATE", BOOL, {.boolValue = false}},
	{"time", "TIME", INT, {.intValue = 1000}},
	{}
};

// Initialize the webManager class using the given webcontent, but without default API functionality
//webManager webCoffee(webContent, contentEntries);

// Initialize the webManager class, with the given webcontent and the given api keywords
webManager webCoffee(webContent, contentEntries, apiKeywords, keywords);

// Static HTML processor for the webManager instance. If more instances are needed, simply create more of theres functions, and add them to the respective webManager instancen in setup. 
String processor(const String &var)  {
	return webCoffee.APIbasedProcessor(var);
}

#define useSerial true

// Start serial
#ifdef useSerial
	void startSerial(uint32_t baudrate = 115200) {
		Serial.begin(baudrate);
	}
#endif

// Start SPIFFS
uint8_t startSPIFFS() {
	#ifdef useSerial
		Serial.print("Starting SPIFFS...");
	#endif
	if (SPIFFS.begin(true)) {
		#ifdef useSerial
			Serial.println("Success!");
		#endif
		return 0;
	} else {
		#ifdef useSerial
			Serial.println("Failed!");
		#endif
		return 1;
	}
}

// Start WiFi in client mode
uint8_t startWIFIclient(const char* ssid, const char* password) {
	#ifdef useSerial
		Serial.print("Starting WiFi. ");
	#endif
	WiFi.mode(WIFI_STA); // Set WiFi mode to client
	WiFi.begin(ssid, password); // Begin WiFi libaray
	#ifdef useSerial
		Serial.print("Connecting");
	#endif
	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		#ifdef useSerial
			Serial.print(".");
		#endif
		delay(500); // Wait for a time
	}
	if (WiFi.status() == WL_CONNECTED) {
		#ifdef useSerial
			Serial.println("Success!");
			Serial.print("Connected to ");
			Serial.println(ssid);
			Serial.print("IP address: ");
			Serial.println(WiFi.localIP());
		#endif
		return 0;
	} else {
		return 1;
	}	
}

// Start MDNS responder
uint8_t startMDNS(const char* hostname) {
	#ifdef useSerial
		Serial.print("Starting MDNS responder...");
	#endif
	if (MDNS.begin(hostname)) {
		#ifdef useSerial
			Serial.println("Success!");
		#endif
		return 0;
	} else {
		#ifdef useSerial
			Serial.println("Failed!");
		#endif
		return 1;
	}
}

// Setup
void setup(void) {
	// Start serial communication
	#ifdef useSerial
		startSerial();
	#endif
	// Start SPIFFS (for managaing website files)
	startSPIFFS();
	// Start WiFi and connect to SSID
	startWIFIclient(ssid, password);
	// Start MDNS responder
	startMDNS("esp32");
	// Add API based HTML processer to webManager
	webCoffee.setHTMLprocessor(processor);
	// Start webManager
	webCoffee.begin();
	#ifdef useSerial
		Serial.println("HTTP server started");
	#endif
}

// Loop
void loop(void) {

}