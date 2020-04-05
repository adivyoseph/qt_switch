#include "mainwindow.h"
#include "config.h"
#include <QApplication>
#include "mqtt.h"
#include <QObject>
//#include "main.moc"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    mqtt myMqtt;

    int rc;
    w.SetMqtt(&myMqtt);
    QObject::connect(&myMqtt, SIGNAL(mqtt_connect_event(int)), &w , SLOT(on_connectEvent(int)));
    QObject::connect(&myMqtt, SIGNAL(mqtt_light_event(int)), &w , SLOT(on_lightEvent(int)));
    QObject::connect(&myMqtt, SIGNAL(mqtt_switch_level(int)), &w , SLOT(on_switchLevelEvent(int)));



    w.show();
    rc = a.exec();

    return rc;
}
