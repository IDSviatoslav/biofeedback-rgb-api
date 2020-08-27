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
    ui->listWidgetDeviceChoice->addItem("Xiaomi");
    ui->listWidgetDeviceChoice->addItem("Another device");

    int result = controller.open();
    if (result == 1) ui->textEditDeviceStatus->setText("CONNECTED");
    else ui->textEditDeviceStatus->setText("CONNECTION FAILED");
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
    int r = ui->spinBoxR->value();
    int g = ui->spinBoxG->value();;
    int b = ui->spinBoxB->value();
    int time = ui->spinBoxTime->value();
    controller.setRgb(r, g, b, time);
}


void MainWindow::on_pushButtonSetBr_clicked()
{
    controller.setBrightness(ui->spinBoxBright->value());
}


void MainWindow::on_checkBoxPowerSwitch_stateChanged(int arg1)
{
    bool state;
    if (arg1 == 0) state = false;
    else state = true;
    controller.setPower(state);
}

void MainWindow::on_pushButtonPickDeviceType_clicked()
{

    controller.setType(ui->listWidgetDeviceChoice->currentItem()->data(0).toString());
}
