#include "mqtt.h"
#include <QtCore>
#include <QDebug>


mqtt::mqtt()
{
    strcpy(host, "localhost");
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, this);
    if(!mosq){
        qDebug() <<  "Error: mosquitto_new failed";

    }


}

mqtt::~mqtt()
{
    if(state){
         mosquitto_disconnect(mosq);
    }
    mosquitto_lib_cleanup();
}

void mqtt::pm_set_state(int state_arg){
    state = state_arg;

}

int mqtt::mqtt_get_state(){
    return state;
}

void mqtt::mqtt_set_port(int port_arg){
    port = port_arg;
}

void mqtt::mqtt_set_host(char *host_arg){
    strcpy(host, host_arg);
}

void my_publish_callback(struct mosquitto *mosq, void *obj, int mid, int reason_code, const mosquitto_property *properties)
{
    //UNUSED(obj);
    //UNUSED(properties);
    //mqtt * pthis = (mqtt * ) obj;

    qDebug() << "publish_callback " << reason_code;

}
void my_disconnect_callback(struct mosquitto *mosq, void *obj, int rc, const mosquitto_property *properties)
{
    //UNUSED(mosq);
    //UNUSED(obj);
    //UNUSED(rc);
    //NUSED(properties);
    mqtt * pthis = (mqtt * ) obj;

    qDebug() << "disconnect_callback " << rc;
    pthis->pm_set_state(0);

}

void my_connect_callback(struct mosquitto *mosq, void *obj, int result, int flags, const mosquitto_property *properties)
{
    int rc = MOSQ_ERR_SUCCESS;

    //UNUSED(obj);
    //UNUSED(flags);
    //UNUSED(properties);
    mqtt * pthis = (mqtt * ) obj;

    qDebug() << "connect_callback " << result;
    pthis->pm_set_state(1);

}

int mqtt::mqtt_connect(){
    int rc;


    mosquitto_connect_v5_callback_set(mosq, my_connect_callback);
    mosquitto_disconnect_v5_callback_set(mosq, my_disconnect_callback);
    mosquitto_publish_v5_callback_set(mosq, my_publish_callback);

    rc = mosquitto_connect_bind_v5(mosq, (const char *) &host, port, 10, (const char *) &host, NULL);
    if(rc>0){
        qDebug() <<  "Error: mosquitto_connect_bind_v5 " << rc;
        return 1;
    }
    else{
        qDebug() <<  "mosquitto_connect_bind_v5";
        state = 1;
    }
    return 0;

}


int mqtt::mqtt_disconnect(){
    int rc = 0;
    if(state){
         rc =mosquitto_disconnect(mosq);
         qDebug() <<  "mosquitto_disconnect rc " << rc;
         state = 0;
    }
    return rc;
}
int mqtt::mqtt_pub(char *subject, std::string * value){
    int mid = 5;
    char msg[8];
    int rc =1;
    int value_len = 0;

    qDebug() <<  "mqtt_pub " << subject << " value " << value->c_str() << "  state  " << state;


    if(state)
    {
        rc = mosquitto_publish_v5(mosq, &mid, (const char *) subject, value->size()+1, value->c_str(), 0, 0, NULL);
        qDebug() <<  "mqtt_pub rc " << rc;
    }
    return rc;
}
