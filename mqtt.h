#ifndef MQTT_H
#define MQTT_H
#include <mqtt_protocol.h>
#include <mosquitto.h>
#include <QObject>

class mqtt
{
public:
    mqtt();
    ~mqtt();
    int mqtt_get_state();
    void mqtt_set_port(int port);
    void mqtt_set_host(char *host);
    int mqtt_connect();
    int mqtt_disconnect();
    int mqtt_pub(char *subject, std::string * value);

    void pm_set_state(int state);

private:
    struct mosquitto *mosq;
    int state = 0;
    int port  = 1883;
    char host[32];
};

#endif // MQTT_H
