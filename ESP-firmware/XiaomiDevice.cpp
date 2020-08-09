#include "XiaomiDevice.h"


XiaomiDevice::XiaomiDevice(const char* netName, const char* pass){
    Serial.begin(115200);
    Serial.print("Strarting...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(netName, pass);
    Serial.println("...done");
    yeelightLamp.setPrintDebug(true);
    yeelightLamp.start();
    //Serial.println("...now here");
}

String XiaomiDevice::getInfo(){
    return "Xiaomi";
}

void XiaomiDevice::setRGB(int r, int g, int b, int duration){
    if (!yeelightLamp.present()) {
        yeelightLamp.find();
        Serial.println("...in setRgb");
    }
    yeelightLamp.setPower(true, smoothTime);
    yeelightLamp.setBrightness(100, smoothTime);
    yeelightLamp.setRGB(r, g, b, smoothTime);
    delay(duration);
}

void XiaomiDevice::setBrightness(int brightness){
    if (!yeelightLamp.present()) {
        yeelightLamp.find();
        Serial.println("...in setBr");
    }
    yeelightLamp.setBrightness(brightness, smoothTime);
}
