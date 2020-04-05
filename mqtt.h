#ifndef MQTT_H
#define MQTT_H
#include <mqtt_protocol.h>
#include <mosquitto.h>
#include <QObject>

class mqtt : public QObject
{
    Q_OBJECT

public:
    mqtt();
    ~mqtt();
    void my_distructor();
    int  mqtt_get_state();
    void mqtt_set_state(int);
    void mqtt_set_port(int port);
    void mqtt_set_host(char *host);
    void mqtt_set_room(QString room);
    QString mqtt_getRoom();
    void mqtt_set_light(QString light);
    QString mqtt_getLight();
    int  mqtt_connect();
    int  mqtt_disconnect();
    void mqtt_pubSwitch();
    void mqtt_pubLevel(int);

signals:
   void mqtt_light_event(int);
   void mqtt_connect_event(int);
   void mqtt_switch_level(int);


private:

    struct mosquitto *mosq;
    int connect_state = 0;
    int port  = 1883;
    char host[32];
    QString stringRoom;
    QString stringLight;

};

#endif // MQTT_H
