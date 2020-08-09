#include <Arduino.h>
#include <yeelight_lamp.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "XiaomiDevice.h"
//#include "Device.h"

const char* ssid     = "LampNet"; 
const char* password = "lamp1234"; 
YeelightLamp yeelight_lamp;

String command;

int rgbBr[3];
int timePeriod;
int brightness = 0;
boolean powerState = false;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  yeelight_lamp.setPrintDebug(false);
  yeelight_lamp.start();
}

void loop() {
  readJSON();

  if (!yeelight_lamp.present()) {
    yeelight_lamp.find();
  }

  if (yeelight_lamp.present()) {
    const unsigned int smooth_time = 500;
    yeelight_lamp.setPower(powerState, smooth_time);
    yeelight_lamp.setBrightness(brightness, smooth_time);
    yeelight_lamp.setRGB(rgbBr[0], rgbBr[1], rgbBr[2], smooth_time);
    delay(timePeriod);
    yeelight_lamp.setPower(false, 500);
  }

}

void readJSON() {
  Serial.print("reading Json \n");
  char CharBuff [160]; 
  const size_t capacity = JSON_OBJECT_SIZE(6);
  boolean reading_complete = false;
  int i = 0;
  while (!reading_complete){
    if (Serial.available() > 0 && i < 160) {
      char ch = char (Serial.read());
      CharBuff [i] = ch;
      if (i==0 && ch!='{'){
        Serial.write("Error READING \n");
        return;
      }
      i++;
      
      if (ch == '}') {
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, CharBuff); 
        if (!error){
          String command = doc["cmd"];

          if (command == "setRgb"){
            timePeriod = doc["time"];
            String color = doc["color"];
            parseColor(color);
            Serial.print("rgb params = \n");
            Serial.print(rgbBr[0]);
            Serial.print(",");
            Serial.print(rgbBr[1]);
            Serial.print(",");
            Serial.print(rgbBr[2]);
            Serial.print ("\n");
          }
          else if (command == "setBr"){
            brightness = doc["brightness"];
            Serial.print("br params = \n");
            Serial.print (brightness);
            Serial.print ("\n");
          }

          else if (command == "setPwr"){
            powerState = doc["state"];
            Serial.print("pwr params = \n");
            Serial.print (powerState);
            Serial.print (" \n");
          }
          reading_complete = true;
        }
        i=0;
      }
    }
  }
}

void parseColor(String str){
  String num;
  int j = 0;
  for (int i = 0; i < 3; i++){
    while(j < str.length()){
      num.concat(str[j]);
      if (str[j]==',') break;  
      j++;
    } 
    rgbBr[i] = atoi(num.c_str());
    num="";
    j++;
  }
}
