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
#include "ESPWebManager.h"

// Define WiFi SSID and password
const char* ssid = "ssid";
const char* password = "password";

// Auto turn off delay
const uint32_t isrTimeout = 600000; // Timeout in ms
// Auto turn off timer
hw_timer_t *isrTurnOffTimer = NULL; // Timer pointer
// ISR mux
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// Coffee maker pin
const uint8_t makerPin = 21;
// Coffee heater pin
const uint8_t heaterPin = 19;

// Number of URLs that the user can access
#define contentEntries 4
// Define all avaliable web pages and resouce elements avaliable to the webserver
const webContentEntry webContent[contentEntries] = {
	{"/", 				"/index.html", 		HTMLfile, 	HTTP_GET},
	{"/myScript.js", 	"/myScript.js", 	RESfile, 	HTTP_GET},
	{"/styles.css", 	"/styles.css", 		RESfile, 	HTTP_GET},
	{"/api", 			"", 				API, 		HTTP_GET | HTTP_POST | HTTP_PUT}
};

// Callback function decleration
void updateCoffeeState();
void updateCoffeeHeater();

// Number of API keywords
#define keywords 2
// Variables accessable from the API
bool coffeeState = false;
bool coffeeHeater = false;
// API keywords
apiKeyword apiKeywords[keywords] = {
	{"coffeeState", "COFFEESTATE", pBOOL, &coffeeState, updateCoffeeState},
	{"coffeeHeater", "COFFEEHEATER", pBOOL, &coffeeHeater, updateCoffeeHeater},
};

// Initialize the webManager class using the given webcontent, but without API functionality
//webManager webCoffee(webContent, contentEntries);
// Initialize the webManager class, with the given webcontent and the given api keywords
webManager webCoffee(webContent, contentEntries, apiKeywords, keywords);

// Static HTML processor for the webManager instance. If more instances are needed, simply create more of theres functions, and add them to the respective webManager instancen in setup. 
String processor(const String &var)  {
	return webCoffee.APIbasedProcessor(var);
}

// Timer interrupt for reattaching interrupt after delay
void IRAM_ATTR autoTurnOffISR() {
	portEXIT_CRITICAL_ISR(&mux);
	digitalWrite(makerPin, LOW);
	coffeeState = false;
	digitalWrite(heaterPin, LOW);
	coffeeHeater = false;
	timerEnd(isrTurnOffTimer);
	isrTurnOffTimer = NULL;
	portEXIT_CRITICAL_ISR(&mux);
	Serial.println("Auto turn off triggered");
}

// Start auto turn off timer
void startAutoTurnOffTimer() {
	if (isrTurnOffTimer == NULL) {
		Serial.println("Starting auto turn off timer");
		isrTurnOffTimer = timerBegin(0, 80, true); // Initialize timer (Timer nr, clock divider, countup), timer counts every microsecond
		timerAttachInterrupt(isrTurnOffTimer, autoTurnOffISR, true); // Attach isr to timer (started timer, function to run, edge triggered
		timerAlarmWrite(isrTurnOffTimer, 1000 * isrTimeout, false); // Set timer alarm (timer, timrout in microseconds, autoreload)
		timerAlarmEnable(isrTurnOffTimer); // Enable alarm
	}
}

// Callback for coffeeState
void updateCoffeeState() {
	Serial.print("Coffee maker state now is: ");
	Serial.println(coffeeState);
	if (coffeeState) {
		Serial.println("Starting coffee maker.");
		digitalWrite(makerPin, HIGH);
		startAutoTurnOffTimer();
	} else {
		Serial.println("Stopping coffee maker.");
		digitalWrite(makerPin, LOW);
	}
}

// Callback for coffeeHeater
void updateCoffeeHeater() {
	Serial.print("Coffee heater state now is: ");
	Serial.println(coffeeHeater);
	if (coffeeHeater) {
		Serial.println("Starting coffee heater.");
		digitalWrite(heaterPin, HIGH);
		startAutoTurnOffTimer();
	} else {
		Serial.println("Stopping coffee heater.");
		digitalWrite(heaterPin, LOW);
	}
}

// Setup
void setup(void) {
	// Start serial communication
	Serial.begin(115200);
	// Start SPIFFS (for managaing website files)
	webCoffee.startSPIFFS();
	// Start WiFi and connect to SSID
	webCoffee.startWIFIclient(ssid, password);
	// Start MDNS responder
	webCoffee.startMDNS("esp32");
	// Add API based HTML processer to webManager
	webCoffee.setHTMLprocessor(processor);
	// Start webManager
	webCoffee.begin();
	Serial.println("HTTP server started");
	pinMode(makerPin, OUTPUT);
	pinMode(heaterPin, OUTPUT);
}

// Loop
void loop(void) {

}