#include "net/mqtt.h"
#include "event.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static mqtt_client_t client;
static Network network;

void initialize_mqtt(void){
    static unsigned char sendbuf[512], readbuf[512];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32_client";
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTConnect(&client, &data);
}

void publish_state(void){
    char *topic = "esp32/state";
    char *msg = "work"; // or "not work" depending on the state

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)msg;
    message.payloadlen = strlen(msg);

    MQTTPublish(&client, topic, &message);
}

int main(void){
    initialize_mqtt();
    while (1){
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}