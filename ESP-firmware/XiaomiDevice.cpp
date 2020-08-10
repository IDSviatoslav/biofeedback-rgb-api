#include "XiaomiDevice.h"

String XiaomiDevice::getInfo(){
    return "Xiaomi";
}

void XiaomiDevice::init(const char* netName, const char* pass){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(netName, pass);
    yeelightLamp.setPrintDebug(false);
    yeelightLamp.start();  
    while (!yeelightLamp.present()) yeelightLamp.find();
}

void XiaomiDevice::setPower(bool state){
  if (!yeelightLamp.present()) yeelightLamp.find();
  yeelightLamp.setPower(state, smoothTime);
}

void XiaomiDevice::setRGB(int r, int g, int b, int duration){
    if (!yeelightLamp.present()) yeelightLamp.find();
    yeelightLamp.setRGB(r, g, b, smoothTime);
    delay(duration);
}

void XiaomiDevice::setBrightness(int brightness){
    if (!yeelightLamp.present()) yeelightLamp.find();
    yeelightLamp.setBrightness(brightness, smoothTime);
}
