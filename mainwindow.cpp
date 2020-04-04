#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mqtt.h"
#include <QDebug>
#include <string>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&configDialog, SIGNAL(configChanged()), this, SLOT(on_configChanged()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetMqtt(mqtt *p_mqtt_arg){
    p_mqtt = p_mqtt_arg;
}

void MainWindow::on_actionExit_triggered()
{
     QApplication::quit();
}

void MainWindow::on_pushButton_clicked()
{
    QString buttonText = "";
    QString number;
    QString topic;
    QMessageBox msgBox;

    if(connect_state) {
        if(lswitchState){
           //currently on
           buttonText = "OFF";  //new state off
           lswitchState = 0;
           ui->pushButton->setText(buttonText);
           topic = QString("%1\\%2").arg(configDialog.getRoomName(), configDialog.getSwitchName());
           p_mqtt->mqtt_pub(&topic, &buttonText);

        }
        else {
           //currently off
           buttonText = "ON"; //new state on
           lswitchState = 1;
           ui->pushButton->setText(buttonText);
           topic = QString("%1\\%2").arg(configDialog.getRoomName(), configDialog.getSwitchName());
           p_mqtt->mqtt_pub(&topic, &buttonText);
           qDebug() << "inial light level " << this->ui->verticalSlider->value();
           number = QString("%1").arg(this->ui->verticalSlider->value());
           topic = QString("%1\\%2_level").arg(configDialog.getRoomName(), configDialog.getSwitchName());
           p_mqtt->mqtt_pub(&topic, &number);
        }
    }
    else {
        msgBox.setText("Warning");
        msgBox.setInformativeText("Not connected");
        msgBox.exec();
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    QString number;
    QString topic;


    if(lswitchState && connect_state){
        //qDebug() <<  "Slider_valueChanged " << value;
        number = QString("%1").arg(value);
        qDebug() <<  "Slider_valueChanged " << value;
        topic = QString("%1\\%2_level").arg(configDialog.getRoomName(), configDialog.getSwitchName());
        p_mqtt->mqtt_pub(&topic, &number);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    std::string buttonText = "";
    if(connect_state){
        buttonText = "Connect";
        p_mqtt->mqtt_disconnect();
        ui->pushButton_2->setText(buttonText.c_str());
        connect_state = 0;
        lswitchState = 0;
        ui->pushButton->setText("OFF");
    }
    else {
        buttonText = "Disconnect";
        p_mqtt->mqtt_connect();
        ui->pushButton_2->setText(buttonText.c_str());
        connect_state = 1;


    }
}

void MainWindow::on_actionConfigure_triggered()
{

    configDialog.show();

}


void MainWindow::on_configChanged(){
    //if(configDialog.getConfigSet()){
        QString text = QString("%1\\%2").arg(configDialog.getRoomName(), configDialog.getSwitchName());

        ui->labelTopic->setText(text );
    //}
}
