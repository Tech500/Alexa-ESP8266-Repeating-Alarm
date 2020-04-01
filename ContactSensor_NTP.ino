/*
 * Example for SinricPro Contactsensor device:
 * - Setup contactsensor device
 * - Support onPowerState to turn on / turn off contactsensor
 * - Checks a contact sensor connected to CONTACT_PIN and send event if state changed
 *
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

//---------------- Code modified by William M. Lucid, AB9NQ 3/29/2020

// Uncomment the following line to enable serial debug output
#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif

#include <Arduino.h>
#include <WiFiUdp.h>  //----------------- Added by WML ---------------------------

#ifdef ESP8266
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32
       #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProContactsensor.h"

//-------------------- Added by WML ---------------

WiFiUDP Udp;
unsigned int localUdpPort = 1313;  // local port to listen for UDP packets
char incomingPacket[255];
char replyPacket[] = "Hi there! Got the message :-)";

#define NTP1 "us.pool.ntp.org"
#define NTP0 "time.nist.gov"

#define TZ "EST+5EDT,M3.2.0/2,M11.1.0/2"

//--------------------- End of added cose ---------

int DOW, MONTH, DATE, YEAR, HOUR, MINUTE, SECOND;

char strftime_buf[64];

String dtStamp(strftime_buf);

int lc = 0;
time_t tnow = 0;

#define WIFI_SSID       "Removed"    
#define WIFI_PASS       "Removed"
#define APP_KEY         "Removed"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET      "Removed"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define CONTACT_ID      "Removed"    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define BAUD_RATE       9600                     // Change baudrate to your need
#define CONTACT_PIN     4                   // PIN where contactsensor is connected to              
                                              // LOW  = contact is open
                                              // HIGH = contact is closed

#define pass   2       //If not getting annoucement from "Alexa" and sound effect; incrementally, adjust this value.   Increasing or decreasing this vpass value.


bool myPowerState = true;
bool actualContactState;
// assume device is turned on
bool lastContactState = false;
unsigned long lastChange = 0;

int flag; //added by WML

/**
 * @brief Checks contactsensor connected to CONTACT_PIN
 *
 * If contactsensor state has changed, send event to SinricPro Server
 * state from digitalRead():
 *      HIGH = contactsensor is closed
 *      LOW  = contactsensor is open
 */
void handleContactsensor() {
  if (!myPowerState) return;                            // if device switched off...do nothing

  unsigned long actualMillis = millis();
  if (actualMillis - lastChange < 250) return;          // debounce contact state transitions (same as debouncing a pushbutton)

  actualContactState = digitalRead(CONTACT_PIN);   // read actual state of contactsensor

  if (actualContactState != lastContactState) {         // if state has changed
    Serial.printf("Contactsensor is %s now\r\n", actualContactState?"open":"closed");
    lastContactState = actualContactState;              // update last known state
    lastChange = actualMillis;                          // update debounce time
    SinricProContactsensor &myContact = SinricPro[CONTACT_ID]; // get contact sensor device
    myContact.sendContactEvent(actualContactState);      // send event with actual state
    Serial.println( "actualContactState:  " + (String)actualContactState);
  }
}

/**
 * @brief Callback for setPowerState request
 *
 * @param deviceId      String containing deviceId (useful if this callback used by multiple devices)
 * @param[in] state     bool true=turn on device / false=turn off device
 * @param[out] state    bool true=device turned on / false=device turned off
 * @return true         request handled properly
 * @return false        request can't be handled because some kind of error happened
 */
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), state?"on":"off");
  myPowerState = state;
  return true; // request handled properly
}


// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  IPAddress localIP = WiFi.localIP();
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

// setup function for SinricPro
void setupSinricPro() {
  // add device to SinricPro
  SinricProContactsensor& myContact = SinricPro[CONTACT_ID];

  // set callback function to device
  myContact.onPowerState(onPowerState);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}
  
// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");

  pinMode(CONTACT_PIN, INPUT);

  setupWiFi();
  setupSinricPro();
  
  //--------------------- Added code by WML -------------------

  configTime(0, 0, NTP0, NTP1);
  setenv("TZ", "EST+5EDT,M3.2.0/2,M11.1.0/2", 3);   // this sets TZ to Indianapolis, Indiana
  tzset();

  flag = 1;
  
  //----------- End of added code

}
 
void loop() {

//----------------- Added code by WML ----------------    

    // send back a reply, to the IP address and port we got the packet from
     Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
     Udp.write(replyPacket);
     Udp.endPacket();

     delay(1000);
 
     getDateTime();

     if(flag == 1)
     {
         
         Serial.println("");
         Serial.println(dtStamp);
         Serial.println(""); 

     }

     flag= 0;

    if ((digitalRead(CONTACT_PIN) == LOW) && ((MINUTE % 10) == 0)) 
    {

        if(SECOND < pass)    
        {  
            
		  //---------------- Sinric Pro code ----------------
		  
		  SinricProContactsensor &myContact = SinricPro[CONTACT_ID]; // get contact sensor device
            myContact.sendContactEvent(actualContactState);      // send event with actual state
            Serial.println( "actualContactState:  " + (String)actualContactState);
		  
		  //---------------- End of Sinric Pro code ---------

            Serial.println("");
            Serial.println(dtStamp);
            Serial.println("");
            
            
        }     
                   
    }
    
    //------------------End of sdded code ---------

    handleContactsensor();
    SinricPro.handle();
    
}

String getDateTime()
{
     struct tm *ti;

     tnow = time(nullptr) + 1;
     //strftime(strftime_buf, sizeof(strftime_buf), "%c", localtime(&tnow));
     ti = localtime(&tnow);
     DOW = ti->tm_wday;
     YEAR = ti->tm_year + 1900;
     MONTH = ti->tm_mon + 1;
     DATE = ti->tm_mday;
     HOUR  = ti->tm_hour;
     MINUTE  = ti->tm_min;
     SECOND = ti->tm_sec;

     strftime(strftime_buf, sizeof(strftime_buf), "%a , %m/%d/%Y , %H:%M:%S %Z", localtime(&tnow));
     dtStamp = strftime_buf;
     return (dtStamp);

}
