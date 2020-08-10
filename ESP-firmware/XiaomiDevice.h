#ifndef XIAOMIDEVICE_H
#define XIAOMIDEVICE_H

#include "Device.h"
#include <ESP8266WiFi.h>
#include <yeelight_lamp.h>

class XiaomiDevice : public Device{
    const int smoothTime = 500;
    public:
      YeelightLamp yeelightLamp;
      void init(const char*, const char*) override;
      String getInfo() override;
      void setPower(bool) override;
      void setRGB(int, int, int, int) override;
      void setBrightness(int) override;
};

#endif
