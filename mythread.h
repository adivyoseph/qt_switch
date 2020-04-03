#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QtCore>
#include <mqtt_protocol.h>
#include <mosquitto.h>
#include <QObject>

class MyThread: public QThread
{
    Q_OBJECT
public:
    MyThread();
    ~MyThread();

};

#endif // MYTHREAD_H
