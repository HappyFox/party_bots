/*
 * WebSocketServerAllFunctionsDemo.ino
 *
 *  Created on: 10.05.2018
 *
 */


#include <Arduino.h>
#include <ArduinoJson.h>

#include <FS.h>
#include <LittleFS.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <ServoEasing.h>

#include <FastLED.h>

#include <Automaton.h>
#include "Atm_angry_arm_ani_machine.h"


#define USE_SERIAL Serial


#define LEFT_SERVO_PIN D4 // D4
#define RIGHT_SERVO_PIN D3 // D3
#define RESET_SERVO_PIN D2 // D4

#define MID_POINT 90
#define RESET_POINT 140
#define RESET_SPEED 100

#define SWITCH_PIN D0

#define DATA_PIN 5
//#define DATA_PIN D1
#define NUM_LEDS 2

CRGB leds[NUM_LEDS];

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);


ServoEasing LeftServo;
ServoEasing RightServo;
ServoEasing ResetServo;

Atm_angry_arm_ani_machine angry_arm_ani_machine(LeftServo, RightServo, leds);

void handle_cmd(const JsonDocument& doc){
    if (!strcmp(doc["action"].as<char *>(), "set_state"))
        Serial.print(F("Setting states of : "));
        Serial.println(doc["state"].as<char *>());
        if (!strcmp(doc["state"].as<char *>(), "malfunction")){
            angry_arm_ani_machine.start();
        }
        else if (!strcmp(doc["state"].as<char *>(), "online")){
            angry_arm_ani_machine.stop();
        }
        else if (!strcmp(doc["state"].as<char *>(), "stop")){
            angry_arm_ani_machine.stop();
        }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            //webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
            StaticJsonDocument<512> doc;

            DeserializationError error = deserializeJson(doc, payload);

            if (error){
                Serial.println(F("Failed to parse incoming json"));
                break;
            }

            handle_cmd(doc);
            break;
    }
}


void setup_wifi(){
    StaticJsonDocument<512> doc;

    File ssid_file = LittleFS.open("/ssid.json", "r");

    deserializeJson(doc, ssid_file);

    ssid_file.close();

    JsonArray arr = doc.as<JsonArray>();

    for (JsonObject ssid_entry : arr)  {
        WiFiMulti.addAP(ssid_entry["ssid"], ssid_entry["password"]);
    }
}


void setup_animation() {
    if (LeftServo.attach(LEFT_SERVO_PIN) == INVALID_SERVO) {
        Serial.println(F("Error attaching Left arm servo"));
    }
    if (RightServo.attach(RIGHT_SERVO_PIN) == INVALID_SERVO) {
        Serial.println(F("Error attaching Right arm servo"));
    }
    if (ResetServo.attach(RESET_SERVO_PIN) == INVALID_SERVO) {
        Serial.println(F("Error attaching Reset servo"));
    }

    ResetServo.setSpeed(RESET_SPEED);
    ResetServo.setEasingType(EASE_QUADRATIC_OUT);
    ResetServo.write(MID_POINT);

    angry_arm_ani_machine.trace( (Stream &) Serial);
    angry_arm_ani_machine.begin();
}


void setup() {
    //USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    leds[0] = CRGB::Green;
    leds[1] = CRGB::Green;
    FastLED.show();

    //USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    LittleFS.begin();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1);

    setup_wifi();
    setup_animation();

    pinMode(SWITCH_PIN, INPUT);

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    if(MDNS.begin("bob")) {
        USE_SERIAL.println("MDNS responder started");
    }

    server.serveStatic("/", LittleFS, "/");
    server.begin();

    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
}

unsigned long last_10sec = 0;
unsigned int counter = 0;

void loop() {
    unsigned long t = millis();

    webSocket.loop();
    server.handleClient();
    MDNS.update();
    automaton.run();

    int buttonState = digitalRead(SWITCH_PIN);

    if(buttonState == 0){
        Serial.println("HIT, Stopping machine.");
            angry_arm_ani_machine.stop();
    }

    if((t - last_10sec) > 10 * 1000) {
        counter++;
        bool ping = (counter % 2);
        int i = webSocket.connectedClients(ping);
        USE_SERIAL.printf("%d Connected websocket clients ping: %d\n", i, ping);
        last_10sec = millis();
    }
}
