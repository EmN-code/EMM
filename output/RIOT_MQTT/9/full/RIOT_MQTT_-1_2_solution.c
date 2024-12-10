#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/emcute.h"
#include "net/af.h"
#include "net/ipv6/addr.h"
#include "mqtt.h"
#include "msg.h"
#include "shell.h"
#include "stdio.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define SENSOR_DATA "work"
#define CLIENT_ID "esp32client"
#define TOPIC_PUB "state"

void *publish_state(void *arg) {
    int rc = -1;
    unsigned char sendbuf[128], readbuf[128];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer; 
    MQTTClient client;
    Network network;
    MQTTMessage message;
    
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Failed to connect to network. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Connected to network.\n");
            break;
        }
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("Failed to connect to MQTT broker. Retrying...\n");
            NetworkDisconnect(&network);
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Connected to MQTT broker.\n");
            break;
        }
    }

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)SENSOR_DATA;
    message.payloadlen = strlen(SENSOR_DATA);

    while (1) {
        rc = MQTTPublish(&client, TOPIC_PUB, &message);
        if (rc != 0) {
            printf("Failed to publish message. Retrying...\n");
            MQTTDisconnect(&client);
            NetworkDisconnect(&network);
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Message published: %s\n", SENSOR_DATA);
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    printf("MQTT example application\n");
    
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_state, NULL, "publish");
<br>    return 0;
}