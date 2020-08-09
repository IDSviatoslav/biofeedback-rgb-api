#ifndef XIAOMIDEVICE_H
#define XIAOMIDEVICE_H

#include "Device.h"
#include <ESP8266WiFi.h>
#include <yeelight_lamp.h>

class XiaomiDevice : public Device{
    YeelightLamp yeelightLamp;
    const int smoothTime = 500;
    public:
      XiaomiDevice (const char*, const char*);
      String getInfo() override;
      void setRGB(int, int, int, int) override;
      void setBrightness(int) override;
};

#endif
