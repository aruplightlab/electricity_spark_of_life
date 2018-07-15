/*
 * Electricity Aurora at Manchester MSI
 * ESP8266 + WS2812 + captive server
 */

#define NODE_NAME "aurora"
#define NUM_LEDS  16

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    4
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define DELAY 20
CRGB leds[NUM_LEDS];
CRGBPalette16 palette = PartyColors_p;

int transition = 1;
int brightness = 150;

#define FRAMES_PER_SECOND  120

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

const int led = 13;
uint8_t idx = 0; // rotating index

//String responseHTML = ""
//  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
//  "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
//  "be redirected here.</p></body></html>";

void setup() {
    pinMode ( led, OUTPUT );
    digitalWrite ( led, 0 );

    delay(3000); // 3 second delay for recovery
  
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

    // set master brightness control
    FastLED.setBrightness(brightness);

    testLEDs();
    setColor(100);
 
    Serial.begin ( 115200 );
  
    
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("aurora");
  
    // if DNSServer is started with "*" for domain name, it will reply with
    // provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", apIP);
  
    // replay to all requests with same HTML
    webServer.onNotFound([]() {
      handleRoot();
//      webServer.send(200, "text/html", responseHTML);
    });

    webServer.on ( "/", []() {handleRoot();} );
    webServer.on ( "/white", []() {setColor(1); handleRoot();} );
    webServer.on ( "/red", []() {setColor(2); handleRoot();} );
    webServer.on ( "/green", []() {setColor(3); handleRoot();} );
    webServer.on ( "/blue", []() {setColor(4); handleRoot();} );
    webServer.on ( "/yellow", []() {setColor(5); handleRoot();} );
    webServer.on ( "/orange", []() {setColor(6); handleRoot();} );
    webServer.on ( "/pink", []() {setColor(7); handleRoot();} );
    webServer.on ( "/black", []() {setColor(0); handleRoot();} );
    webServer.on ( "/brighten", []() {brighten(); handleRoot();} );
    webServer.on ( "/dim", []() {dim(); handleRoot();} );
    webServer.on ( "/sinelon", []() {transition=1; handleRoot();} );
    webServer.on ( "/bpm", []() {transition=2; handleRoot();} );
    webServer.on ( "/gradient", []() {transition=3; handleRoot();} );
    webServer.on ( "/random", []() {transition=4; handleRoot();} );
    webServer.on ( "/aiml", []() {setColor(100); handleRoot();} );
    webServer.onNotFound ( handleNotFound );
    webServer.begin();
    Serial.println ( "HTTP server started" );

    
    webServer.begin();
}

void loop() {
    dnsServer.processNextRequest();
    webServer.handleClient();
    // run transition
    if (transition==1) t_sinelon();
    else if (transition==2) t_bpm();
    else if (transition==3) t_gradient();
    else if (transition==4) t_random();
    // reset if reset time has passed
    //if (millis() > reset_time) {
    //  ESP.restart();
    //}
    // send the 'leds' array out to the actual LED strip
    FastLED.show();  
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND); 
    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) { idx++; } // slowly cycle the index variable
    //EVERY_N_MILLISECONDS(5000) { Serial.println(millis()); }
}
