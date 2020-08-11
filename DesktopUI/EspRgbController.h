#ifndef ESPRGBCONTROLLER_H
#define ESPRGBCONTROLLER_H

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

class EspController{
private:
    //EspRgbApi();
public:
    //EspController();
    QString r;
    QString g;
    QString b;
    QString time;
    QString brightness;
    QVector <QString> timelineCommands;
    QSerialPort device;
    QJsonArray colors;
    bool deviceIsConnected;
    QString deviceType;

    int open();
    void setRgb(int r, int g, int b, int time);
    void setBrightness(int brightness);
    void setPower(bool powerState);
    void setType(QString seviceType);
};

#endif // ESPRGBCONTROLLER_H
