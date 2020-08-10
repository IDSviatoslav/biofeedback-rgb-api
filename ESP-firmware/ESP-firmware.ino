#include <Arduino.h>
#include <yeelight_lamp.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "XiaomiDevice.h"

const char* ssid     = "LampNet"; 
const char* password = "lamp1234"; 

Device *device;

int rgbBr[3];
int timePeriod;
int brightness = 0;
boolean powerState = false;
String deviceType = "none";

void setup() {
  Serial.begin(115200);
  String test;
  while(deviceType == "none"){
    test = readJSON();
    Serial.println(test);
  }
  device = deviceFactory(deviceType);
  device->init(ssid, password);
}

void loop() {  
  String command = readJSON();
  executeCmd(command);
}


void executeCmd(String cmd){
  if (cmd == "setRgb") device->setRGB(rgbBr[0], rgbBr[1], rgbBr[2], timePeriod);
  else if (cmd == "setBr") device->setBrightness(brightness);
  else if (cmd == "setPwr") device->setPower(powerState);
  else Serial.println(cmd);
}

String readJSON() {
  char CharBuff [160]; 
  const size_t capacity = JSON_OBJECT_SIZE(6);
  boolean reading_complete = false;
  int i = 0;
  while (!reading_complete){
    if (i > 160) reading_complete = true;
    if (Serial.available() > 0 && i < 160) {
      char ch = char (Serial.read());
      CharBuff [i] = ch;
      if (i==0 && ch!='{'){
        return "json reading error - wrong input";
      }
      i++;
      
      if (ch == '}') {
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, CharBuff); 
        if (!error){
          String command = doc["cmd"];

          if(command == "setDevType"){
              deviceType = doc["devType"].as<String>();
              Serial.print ("devType = ");
              Serial.println(deviceType);
          }
          else if (command == "setRgb"){
            timePeriod = doc["time"];
            String color = doc["color"];
            parseColor(color);
            Serial.print("rgb params = ");
            Serial.print(rgbBr[0]);
            Serial.print(",");
            Serial.print(rgbBr[1]);
            Serial.print(",");
            Serial.println(rgbBr[2]);
          }
          else if (command == "setBr"){
            brightness = doc["brightness"];
            Serial.print("br params = ");
            Serial.println(brightness);
          }

          else if (command == "setPwr"){
            powerState = doc["state"];
            Serial.print("pwr params = ");
            Serial.println(powerState);
          }
          reading_complete = true;
          return command;
        }
        else return error.c_str();
      }
    }
  }
  return "reading unsucessful";
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

Device *deviceFactory(String deviceType){
    if(deviceType == "Xiaomi") return new XiaomiDevice(); 
}
