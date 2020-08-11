#include <Arduino.h>
#include <yeelight_lamp.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "XiaomiDevice.h"

const char* ssid     = "LampNet"; 
const char* password = "lamp1234"; 

Device *device;

//int rgbBr[3];
//int timePeriod;
//int brightness = 0;
//boolean powerState = false;

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
  if (cmd == "setRgb") device->setRGB(device->RGB[0], device->RGB[1], device->RGB[2], device->timePeriod);
  else if (cmd == "setBr") device->setBrightness(device->brightness);
  else if (cmd == "setPwr") device->setPower(device->powerState);
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
            device->timePeriod = doc["time"];
            String color = doc["color"];
            parseColor(color);
            Serial.print("rgb params = ");
            Serial.print(device->RGB[0]);
            Serial.print(",");
            Serial.print(device->RGB[1]);
            Serial.print(",");
            Serial.println(device->RGB[2]);
          }
          else if (command == "setBr"){
            device->brightness = doc["brightness"];
            Serial.print("br params = ");
            Serial.println(device->brightness);
          }

          else if (command == "setPwr"){
            device->powerState = doc["state"];
            Serial.print("pwr params = ");
            Serial.println(device->powerState);
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
    device->RGB[i] = atoi(num.c_str());
    num="";
    j++;
  }
}

Device *deviceFactory(String deviceType){
    if(deviceType == "Xiaomi") return new XiaomiDevice(); 
}
