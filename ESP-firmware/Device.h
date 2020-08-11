#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>

class Device{
    public:
        int RGB[3] = {0, 0, 0};
        int timePeriod = 0;
        int brightness = 0;
        boolean powerState = false;
        virtual String getInfo() = 0;
        virtual void init(const char*, const char*) = 0;
        virtual void setPower(bool) = 0;
        virtual void setRGB(int, int, int, int) = 0;
        virtual void setBrightness(int) = 0;
};

#endif
