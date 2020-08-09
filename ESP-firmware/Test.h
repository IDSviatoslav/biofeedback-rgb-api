#ifndef TEST_H
#define TEST_H

//#include <string.h>
//using std::string;

class Test{
    public:
        //virtual const string& getInfo() const = 0;
        virtual void setRGB(int, int, int, int) = 0;
        virtual void setBrightness(int) = 0;
};

#endif