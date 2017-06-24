////////////////////////////////////////////////////////////////////////////////////////////
// Longside Radio Display Sign /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//
// This script relies heavily on example code from the libraries below! 
// 
// This works here but I accept no responsibility for broken hardware, fire, explosions etc! 
////////////////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <SPI.h>

// If you have are using the individual modules then the below lib is fine,  if you have the 
// integrated 4 matrixes already linked together or if you text is coming out sideways then
// use the https://github.com/targetblank/MAX7219LedMatrix fork as the main one currently 
// doesn't support rotation. If you do use the fork you need to set the rotation to true in the 
// config section. 

#include "LedMatrix.h"  // https://github.com/squix78/MAX7219LedMatrix

////////////////////////////////////////////////////////////////////////////////////////////
// WIRING //////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// If you are using a NodeMCU or Wemos D1 mini style board then connect : 
// VCC --> 5V PIN
// GND --> GND PIN
// DIN --> D7 PIN
// CS  --> D4 PIN
// CLK --> D5 PIN 
////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////
// CONFIG SECTION //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// CHANGEME: This is the number of MAX7219 LED matrix devices 
#define NUMBER_OF_DEVICES 8
//
// CHANGEME: This is the default message on boot,  if you leave this blank then it will loop
//           the IP address,  if you put in a couple of spaces it will be blank after the IP
//           address is shown
String defaultMessage = "Longside Radio: Where Variety Matters - www.longsideradio.co.uk";
//
// CHANGEME: Captive portal name
const char* capName = "LongsideLED";
//
// CHANGEME: Webpage title
String webpageTitle = "Longside Radio LED sign";
//
// CHANGEME: If you need to rotate the display 90 degrees set this to true else false
String ledRotate = "false";
/////////////////////////////////////////////////////////////////////////////////////////////


// Setup webserver and matrixes
String displayMessage = defaultMessage;
#define CS_PIN 2
ESP8266WebServer server(80);
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);


//find my IP address
const char* myIpAddress(void);
extern char* itoa(int a, char* buffer, unsigned char radix);
const char* myIpAddress(void)
{
  IPAddress myIp = WiFi.localIP();
  static char buf[24] = "";
  char num[4];
  itoa(myIp[0],num,10);
  strcat(buf,num); strcat(buf,".");
  itoa(myIp[1],num,10);
  strcat(buf,num); strcat(buf,".");
  itoa(myIp[2],num,10);
  strcat(buf,num); strcat(buf,".");
  itoa(myIp[3],num,10);
  strcat(buf,num);
  return buf;
}

//Web handling
void handleRoot() {
  String msg = server.arg("msg");
  Serial.println(msg);
  if (msg.length() > 0) {  
  displayMessage = msg;
  }
        server.send(200, "text/html", "<html><head><title>"
        +webpageTitle+
        "</title>"
        "<meta name=\"viewport\" content=\"width=device-width, target-densitydpi=device-dpi, initial-scale=0, maximum-scale=1, user-scalable=yes\" />"
        "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/milligram/1.3.0/milligram.css\">"
        "<link rel=\"stylesheet\" href=\"//fonts.googleapis.com/css?family=Roboto:300,300italic,700,700italic\">"
        "<link rel=\"stylesheet\" href=\"//cdn.rawgit.com/necolas/normalize.css/master/normalize.css\">"
        "</head><body>" 
        "<p>"
        "<center>"
        "<h3>Currently displaying:</h3><h2>"
         +displayMessage+      
        "</h2><h3>What do you want to say?</h3>"      
        "<form action='/'><p><input type='text' name='msg' size=50 autofocus> <input type='submit' value='Submit'>"
        "<a class=\"button button-outline\" href=\"/clear\" title=\"Clear Display\">Clear Display</a>"
        "<a class=\"button button-outline\" href=\"/default\" title=\"Default Message\">Default Message</a>"
        "</form></center>"
        "</body></html>");
}

void handleNotFound(){
        String message = "File Not Found\n\n";
        message += "URI: ";
        message += server.uri();
        message += "\nMethod: ";
        message += (server.method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += server.args();
        message += "\n";
        for (uint8_t i=0; i<server.args(); i++) {
                message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
        }
        server.send(404, "text/plain", message);
}

void handleClear(){
   displayMessage = "    ";
   server.send(200, "text/html", "<html><head><title>"
        +webpageTitle+
        "</title>"
        "<meta name=\"viewport\" content=\"width=device-width, target-densitydpi=device-dpi, initial-scale=0, maximum-scale=1, user-scalable=yes\" />"
        "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/milligram/1.3.0/milligram.css\">"
        "<link rel=\"stylesheet\" href=\"//fonts.googleapis.com/css?family=Roboto:300,300italic,700,700italic\">"
        "<link rel=\"stylesheet\" href=\"//cdn.rawgit.com/necolas/normalize.css/master/normalize.css\">"
        "</head><body>" 
        "<p>"
        "<center>"
        "<h3>What do you want to say?</h3>"
        "<form action='/'><p><input type='text' name='msg' size=50 autofocus> <input type='submit' value='Submit'>"
        "<a class=\"button button-outline\" href=\"/default\" title=\"Default Message\">Default Message</a>"
        "</form></center>"
        "</body></html>");

}


void handleDefault(){
   displayMessage = defaultMessage;
   server.send(200, "text/html", "<html><head><title>"
        +webpageTitle+
        "</title>"
        "<meta name=\"viewport\" content=\"width=device-width, target-densitydpi=device-dpi, initial-scale=0, maximum-scale=1, user-scalable=yes\" />"
        "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/milligram/1.3.0/milligram.css\">"
        "<link rel=\"stylesheet\" href=\"//fonts.googleapis.com/css?family=Roboto:300,300italic,700,700italic\">"
        "<link rel=\"stylesheet\" href=\"//cdn.rawgit.com/necolas/normalize.css/master/normalize.css\">"
        "</head><body>" 
        "<p>"
        "<center>"
        "<h3>Currently displaying:</h3><h2>"
        +displayMessage+ 
        "<h3>What do you want to say?</h3>"
        "<form action='/'><p><input type='text' name='msg' autofocus> <input type='submit' value='Submit'>"
        "<a class=\"button button-outline\" href=\"/clear\" title=\"Clear Display\">Clear Display</a>"
        "</form></center>"
        "</body></html>");

}

//Setup sketch
void setup() {
  WiFiManager wifiManager;
  wifiManager.autoConnect(capName);
  ledMatrix.init();
  ledMatrix.setText(myIpAddress());
  
  
  server.on("/", handleRoot);
  server.on("/clear", handleClear);
  server.on("/default", handleDefault);
  server.onNotFound(handleNotFound);
//If you go to this page it will wipe all settings and restart the ESP
//Useful for testing but probably best left commented out in production
//  server.on("/zz9reset!", [] (){
//            server.send(200, "text/plain", "ZMG?? where am I?");
//            WiFi.disconnect();
//            ESP.restart();
//            });
  
  server.begin();
}


//Main loop
void loop() {
  server.handleClient();
  ledMatrix.clear();
  ledMatrix.setRotation(ledRotate);
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  ledMatrix.setNextText(displayMessage); 

}
