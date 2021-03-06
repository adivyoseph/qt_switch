#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "mqtt.h"
#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetMqtt(mqtt *p_mqtt);

private slots:
    void on_actionExit_triggered();

    void on_pushButton_clicked();

    void on_verticalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_actionConfigure_triggered();

    void on_configChanged();

    void on_connectEvent(int state);

    void on_lightEvent(int);

    void on_switchLevelEvent(int);

private:
    Ui::MainWindow *ui;
    Config configDialog;
    int connect_state = 0;
    int lswitchState = 0;
    int lightLevel = 5;
    int no_level_update;
    mqtt *p_mqtt;

};

#endif // MAINWINDOW_H
