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

//light switch button pressed
void MainWindow::on_pushButton_clicked()
{
    QString number;
    QMessageBox msgBox;

    if(connect_state) {
        //just send switch touched message
        //and update level

        p_mqtt->mqtt_pubSwitch();
        p_mqtt->mqtt_pubLevel(this->ui->verticalSlider->value());

    }
    else {
        msgBox.setText("Warning");
        msgBox.setInformativeText("Not connected");
        msgBox.exec();
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    if(no_level_update ){
        no_level_update =  0;
    }
    else {
        if(connect_state && lswitchState){
            qDebug() <<  "Slider_valueChanged " << value;
            p_mqtt->mqtt_pubLevel(value);
        }
    }

}



//Connect button
void MainWindow::on_pushButton_2_clicked()
{
     if(connect_state){


         p_mqtt->mqtt_disconnect();

    }
    else {
         //update config
         QString name = QString(configDialog.getRoomName());
         QString light = QString(configDialog.getSwitchName());
         p_mqtt->mqtt_set_room(QString(configDialog.getRoomName()));
         p_mqtt->mqtt_set_light(QString(configDialog.getSwitchName())) ;


        p_mqtt->mqtt_connect();
    }
}

void MainWindow::on_actionConfigure_triggered()
{

    configDialog.show();

}


void MainWindow::on_configChanged(){
    //if(configDialog.getConfigSet()){
        QString text = QString("%1/%2").arg(configDialog.getRoomName(), configDialog.getSwitchName());

        ui->labelTopic->setText(text );

    //}
}


void MainWindow::on_connectEvent(int state){
    qDebug() << "on_connectEvent" << state;
    if(state){
        ui->pushButton_2->setText("Disconnect");
        connect_state = 1;
        lswitchState = 0;
        ui->pushButton->setText("OFF");
    }
    else {
        ui->pushButton_2->setText("Connect");
        connect_state = 0;
        lswitchState = 0;
    }

}

void MainWindow::on_lightEvent(int lightState){
    if(lightState != lswitchState){
        lswitchState = lightState;
        if(lightState){
            ui->pushButton->setText("ON");
        }
        else{
            ui->pushButton->setText("OFF");
        }
    }

}

void MainWindow::on_switchLevelEvent(int level){
    //if(lswitchState){
    qDebug() << "on_switchLevelEven " << level;
        if(ui->verticalSlider->value() != level){
            no_level_update = 1;

         ui->verticalSlider->setValue(level);
        }
    //}
}
