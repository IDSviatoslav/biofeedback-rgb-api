#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>

class Device{
    public:
        virtual String getInfo() = 0;
        virtual void init(const char*, const char*) = 0;
        virtual void setPower(bool) = 0;
        virtual void setRGB(int, int, int, int) = 0;
        virtual void setBrightness(int) = 0;
};

#endif
