#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:


private slots:
    void on_sliderR_sliderMoved();

    void on_sliderG_sliderMoved();

    void on_sliderB_sliderMoved();

    void on_spinBoxTime_valueChanged(const QString &arg1);

    void on_spinBoxBright_valueChanged(const QString &arg1);

    void on_checkBoxPowerSwitch_stateChanged(int arg1);

    void on_pushButtonClear_clicked();

    void on_pushButtonSetRgb_clicked();

    void on_pushButtonSetBr_clicked();

    void on_pushButtonPickDeviceType_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
