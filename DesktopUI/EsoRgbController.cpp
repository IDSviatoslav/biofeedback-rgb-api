#include "EspRgbController.h"

/*EspController::EspController()
{
}*/

int EspController::open()
{
    QList<QSerialPortInfo> allPorts = QSerialPortInfo::availablePorts();
    for (int i = 0; i < allPorts.length();i++){
        qDebug() << allPorts.value(i).productIdentifier();
        qDebug() << allPorts.value(i).vendorIdentifier();
        if (allPorts.value(i).productIdentifier() == 60000 && allPorts.value(i).vendorIdentifier() == 4292){
            device.setPortName(allPorts.value(i).portName());
            device.open(QIODevice::ReadWrite);
            device.setBaudRate(115200);
            device.setDataBits(QSerialPort::Data8);
            device.setParity(QSerialPort::NoParity);
            device.setStopBits(QSerialPort::OneStop);
            qDebug() << "CONNECTED";
            return 1;
        }
    }
    return 0;
}

void EspController::setRgb(int r, int g, int b, int time)
{
    QJsonObject commandObj;
    QString color = QString::number(r) + " , " + QString::number(g) + " , " + QString::number(b);
    commandObj.insert("cmd", "setRgb");
    commandObj.insert("time", QJsonValue::fromVariant(time));
    commandObj.insert("color", QJsonValue::fromVariant(color));
    QJsonDocument doc(commandObj);
    QString command(doc.toJson(QJsonDocument::Compact));

    if (device.isWritable()){
        device.write(command.toStdString().c_str());
        qDebug() << "rgb values sent";
        qDebug() << command;
    }

    if (device.isReadable()){
        qDebug() << "Trying to read";
        QString readMessage = QString(device.readAll());
        qDebug() << readMessage;
    }
}

void EspController::setBrightness(int brightness)
{
    QJsonObject commandObj;
    commandObj.insert("cmd", "setBr");
    commandObj.insert("brightness", QJsonValue::fromVariant(brightness));
    QJsonDocument doc(commandObj);
    QString command(doc.toJson(QJsonDocument::Compact));
    if (device.isWritable()){
        device.write(command.toStdString().c_str());
        qDebug() << "brightness values sent";
        qDebug() << brightness;
    }

    if (device.isReadable()){
        qDebug() << "Trying to read";
        QString readMessage = QString(device.readAll());
        qDebug() << readMessage;
    }
}

void EspController::setPower(bool powerState)
{
    QJsonObject commandObj;
    commandObj.insert("cmd", "setPwr");
    commandObj.insert("state", powerState);
    QJsonDocument doc(commandObj);
    QString command(doc.toJson(QJsonDocument::Compact));
    if (device.isWritable()){
        device.write(command.toStdString().c_str());
        qDebug() << "power values sent";
        qDebug() << powerState;
    }

    if (device.isReadable()){
        qDebug() << "Trying to read";
        QString readMessage = QString(device.readAll());
        qDebug() << readMessage;
    }
}

void EspController::setType(QString deviceType)
{
    QJsonObject commandObj;
    commandObj.insert("cmd", "setDevType");
    commandObj.insert("devType", deviceType);
    QJsonDocument doc(commandObj);
    QString command(doc.toJson(QJsonDocument::Compact));
    if (device.isWritable()){
        device.write(command.toStdString().c_str());
        qDebug() << "devType sent";
        qDebug() << deviceType;
    }

    if (device.isReadable()){
        qDebug() << "Trying to read";
        QString readMessage = QString(device.readAll());
        qDebug() << readMessage;
    }
}
