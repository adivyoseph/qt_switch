#include "mainwindow.h"
#include <QApplication>
#include "mqtt.h"
#include <QObject>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    mqtt myMqtt;

    int rc;
    w.SetMqtt(&myMqtt);

    w.show();
    rc = a.exec();

    return rc;
}
