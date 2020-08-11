#include "XiaomiDevice.h"

//XiaomiDevice::XiaomiDevice(){
  //brightness = 0;
  //for (int i = 0;i<3;i++) RGB[i] = 0;
 // powerState = false;
//}

String XiaomiDevice::getInfo(){
    return "Xiaomi";
}

void XiaomiDevice::init(const char* netName, const char* pass){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(netName, pass);
    yeelightLamp.setPrintDebug(false);
    yeelightLamp.start();  
    while (!yeelightLamp.present()) yeelightLamp.find();
    yeelightLamp.setPower(powerState, smoothTime);
    yeelightLamp.setBrightness(brightness, smoothTime);
    yeelightLamp.setRGB(255, 255, 255, smoothTime);
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

void XiaomiDevice::setBrightness(int brightnessValue){
    if (!yeelightLamp.present()) yeelightLamp.find();
    yeelightLamp.setBrightness(brightnessValue, smoothTime);
}
