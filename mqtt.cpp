#include "mqtt.h"
#include <QtCore>
#include <QDebug>

#define UNUSED(expr) (void)(expr)

mqtt::mqtt()
{
    strcpy(host, "localhost");
    mosquitto_lib_init();
    mosq = mosquitto_new(nullptr, true, this);
    if(!mosq){
        qDebug() <<  "Error: mosquitto_new failed";

    }
}

mqtt::~mqtt()
{
    if(connect_state){
         mosquitto_disconnect(mosq);
    }
    mosquitto_lib_cleanup();
}

void mqtt::mqtt_set_state(int state_arg){
    connect_state = state_arg;

}

int mqtt::mqtt_get_state(){
    return connect_state;
}

void mqtt::mqtt_set_port(int port_arg){
    port = port_arg;
}

void mqtt::mqtt_set_host(char *host_arg){
    strcpy(host, host_arg);
}

void my_publish_callback(struct mosquitto *mosq, void *obj, int mid, int reason_code, const mosquitto_property *properties)
{
    UNUSED(mosq);
    UNUSED(obj);
    UNUSED(mid);
    UNUSED(properties);

    qDebug() << "publish_callback " << reason_code;

}
void my_disconnect_callback(struct mosquitto *mosq, void *obj, int rc, const mosquitto_property *properties)
{
    UNUSED(mosq);
    //UNUSED(obj);
    //UNUSED(rc);
    UNUSED(properties);
    mqtt * pthis = (mqtt * ) obj;

    qDebug() << "disconnect_callback " << rc;
    pthis->mqtt_set_state(0);
    emit pthis->mqtt_connect_event(0);

}

void my_connect_callback(struct mosquitto *mosq, void *obj, int result, int flags, const mosquitto_property *properties)
{
    UNUSED(mosq);
    //UNUSED(obj);
    UNUSED(flags);
    UNUSED(properties);
    mqtt * pthis = (mqtt * ) obj;

    qDebug() << "connect_callback " << result;
    pthis->mqtt_set_state(1);
    emit pthis->mqtt_connect_event(1);

}

void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg, const mosquitto_property *properties){
    UNUSED(mosq);
    UNUSED(properties);
    char *pchar = (char *) msg->payload;
    mqtt * pthis = (mqtt * ) obj;
    QString topicLight;
    QString topicLevel;

    topicLight = QString("%1\\%2_light").arg(pthis->mqtt_getRoom(), pthis->mqtt_getLight());
    topicLevel = QString("%1\\%2_level").arg(pthis->mqtt_getRoom(), pthis->mqtt_getLight());


    qDebug() << "message_callback " << msg->topic << "len " << msg->payloadlen;
    //qDebug() << "    topicSwitch  " << topicLight.toStdString().c_str();
    //only get here when connected
    //only subscribe to light events

    // topic
    if( strcmp(topicLight.toStdString().c_str(), msg->topic) == 0){
        if(pchar[0] == '1'){
            emit pthis->mqtt_light_event(1);
            qDebug() << "   light on";
        }
        else if(pchar[0] == '0'){
            emit pthis->mqtt_light_event(0);
            qDebug() << "   light off";
        }
    }
    else if( strcmp(topicLevel.toStdString().c_str(), msg->topic) == 0){
        int value = atoi(pchar);
        if(value <= 100){
            qDebug() << "   level " << value;
            emit pthis->mqtt_switch_level(value);
        }
    }


}




int mqtt::mqtt_connect(){
    int rc;
    QString topicLight;

    topicLight = QString("%1\\%2_light").arg(stringRoom, stringLight);
    QString topicLevel = QString("%1\\%2_level").arg(stringRoom, stringLight);

    mosquitto_connect_v5_callback_set(mosq, my_connect_callback);
    mosquitto_disconnect_v5_callback_set(mosq, my_disconnect_callback);
    mosquitto_publish_v5_callback_set(mosq, my_publish_callback);
    mosquitto_message_v5_callback_set(mosq, my_message_callback);

    rc = mosquitto_connect_bind_v5(mosq, (const char *) &host, port, 60, (const char *) &host, nullptr);
    if(rc>0){
        qDebug() <<  "Error: mosquitto_connect_bind_v5 " << rc;
        return 1;
    }
    else{
        qDebug() <<  "mosquitto_connect_bind_v5";
        connect_state = 1;
        rc = mosquitto_subscribe_v5(mosq, nullptr,  topicLight.toStdString().c_str(), 0, 0, nullptr);
        rc = mosquitto_subscribe_v5(mosq, nullptr,  topicLevel.toStdString().c_str(), 0, 0, nullptr);
        mosquitto_loop_start(mosq);
    }
    return 0;

}


int mqtt::mqtt_disconnect(){
    int rc = 0;
    if(connect_state){
         rc =mosquitto_disconnect(mosq);
         qDebug() <<  "mosquitto_disconnect rc " << rc;
         connect_state = 0;
         mosquitto_loop_stop(mosq, false);
    }
    return rc;
}

void mqtt::mqtt_pubSwitch(){
    int mid = 5;
    int rc =1;
    QString topic = QString("%1\\%2_switch").arg(stringRoom, stringLight);
    QString msg = "1";

    qDebug() <<  "mqtt_pubSwitch state  " << connect_state;


    if(connect_state)
    {
        rc = mosquitto_publish_v5(mosq, &mid, topic.toStdString().c_str(), msg.size(), msg.toStdString().c_str(), 0, 0, nullptr);
        qDebug() <<  "mqtt_pub rc " << rc;
    }
}


void mqtt::mqtt_pubLevel(int value){
    int mid = 5;
    int rc =1;
    QString topic = QString("%1\\%2_level").arg(stringRoom, stringLight);
    QString number =QString("%1").arg(value);

    qDebug() <<  "mqtt_publevel " << topic.toStdString().c_str() << " " << number.toStdString().c_str() << " state  " << connect_state;


    if(connect_state)
    {
        rc = mosquitto_publish_v5(mosq, &mid, topic.toStdString().c_str(), number.size(), number.toStdString().c_str(), 0, 0, nullptr);
        qDebug() <<  "mqtt_pub rc " << rc;
    }
}

void mqtt::mqtt_set_room(QString room){
    stringRoom = room;
}

void mqtt::mqtt_set_light(QString light){
    stringLight = light;
}

QString mqtt::mqtt_getRoom(){
    return stringRoom;
}
QString mqtt::mqtt_getLight(){
    return stringLight;
}

