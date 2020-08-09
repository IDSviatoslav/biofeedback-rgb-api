#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    r = QString::number(ui->sliderR->value());
    g = QString::number(ui->sliderG->value());
    b = QString::number(ui->sliderB->value());


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
        }

    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if (device.isOpen()) device.close();
}


void MainWindow::on_sliderR_sliderMoved()
{
    r = QString::number(ui->sliderR->value());
    ui->displayColor->setStyleSheet("QLabel{background-color:rgb("+ r +", "+ g +", "+ b +");}");
}

void MainWindow::on_sliderG_sliderMoved()
{
    g = QString::number(ui->sliderG->value());
    ui->displayColor->setStyleSheet("QLabel{background-color:rgb("+ r +", "+ g +", "+ b +");}");
}

void MainWindow::on_sliderB_sliderMoved()
{
    b = QString::number(ui->sliderB->value());
    ui->displayColor->setStyleSheet("QLabel{background-color:rgb("+ r +", "+ g +", "+ b +");}");
}

void MainWindow::on_pushButtonSetRgb_clicked()
{
    /*if (device.isWritable()){
        for (int i = 0; i < timelineCommands.size(); i++){
            ui->textEdit->append(timelineCommands.at(i));
            ui->textEdit->append("\n");
            device.write(timelineCommands.at(i).toStdString().c_str());
            qDebug() << "rgb values sent";
            qDebug() << timelineCommands.at(i);
        }
    }*/

    qDebug() << "Entered setRgb fun";

    QJsonObject commandObj;
    QString color = r + " , " + g + " , " + b;
    commandObj.insert("cmd", "setRgb");
    commandObj.insert("time", QJsonValue::fromVariant(time));
    commandObj.insert("color", QJsonValue::fromVariant(color));
    QJsonDocument doc(commandObj);
    QString command(doc.toJson(QJsonDocument::Compact));

    if (device.isWritable()){
        ui->textEdit->append(command);
        ui->textEdit->append("\n");
        device.write(command.toStdString().c_str());
        qDebug() << "rgb values sent";
        qDebug() << command;
    }

    if (device.isReadable()){
        qDebug() << "Trying to read";
        QString readMessage = QString(device.readAll());
        ui->nodeMCUAnswer->setText(readMessage);
        qDebug() << readMessage;
    }
}

void MainWindow::on_pushButtonAddPoint_clicked()
{
    /*QJsonObject commandObj;
    QString color = r + " , " + g + " , " + b;
    commandObj.insert("cmd", "setRgb");
    commandObj.insert("time", QJsonValue::fromVariant(time));
    commandObj.insert("color", QJsonValue::fromVariant(color));
    QJsonDocument doc(commandObj);
    QString command(doc.toJson(QJsonDocument::Compact));
    timelineCommands.append(command);*/
}

void MainWindow::on_spinBoxTime_valueChanged(const QString &arg1)
{
    time = arg1;
}

void MainWindow::on_pushButtonSendCommand_clicked()
{
    QString command = ui->textEditSendCommand->toPlainText();

    if (device.isWritable()){
        device.write(command.toStdString().c_str());
        qDebug() << "command sent";
    }
}

void MainWindow::on_pushButtonSetBr_clicked()
{
    qDebug() << "Entered setBr fun";
    QJsonObject commandObj;
    commandObj.insert("cmd", "setBr");
    commandObj.insert("brightness", QJsonValue::fromVariant(brightness));
    QJsonDocument doc(commandObj);
    QString command(doc.toJson(QJsonDocument::Compact));
    if (device.isWritable()){
        device.write(command.toStdString().c_str());
        qDebug() << "brightness values sent";
        qDebug() << brightness;
        ui->textEdit->append(command);
        ui->textEdit->append("\n");
    }

    if (device.isReadable()){
        qDebug() << "Trying to read";
        QString readMessage = QString(device.readAll());
        ui->nodeMCUAnswer->setText(readMessage);
        qDebug() << readMessage;
    }
}

void MainWindow::on_spinBoxBright_valueChanged(const QString &arg1)
{
    brightness = arg1;
}

void MainWindow::on_checkBoxPowerSwitch_stateChanged(int arg1)
{
    bool state;
    QJsonObject commandObj;
    commandObj.insert("cmd", "setPwr");
    if (arg1 == 0) state = false;
    else state = true;
    commandObj.insert("state", state);
    QJsonDocument doc(commandObj);
    QString command(doc.toJson(QJsonDocument::Compact));
    if (device.isWritable()){
        device.write(command.toStdString().c_str());
        qDebug() << "power values sent";
        qDebug() << state;
        ui->textEdit->append(command);
        ui->textEdit->append("\n");
    }

    if (device.isReadable()){
        qDebug() << "Trying to read";
        QString readMessage = QString(device.readAll());
        ui->nodeMCUAnswer->setText(readMessage);
        qDebug() << readMessage;
    }
}

void MainWindow::on_pushButtonClear_clicked()
{
    ui->textEdit->clear();
}
