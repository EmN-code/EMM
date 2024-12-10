#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "status"
#define STACK_SIZE THREAD_STACKSIZE_DEFAULT

static char stack[STACK_SIZE];

void *mqtt_thread(void *arg) {
    (void)arg;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    int rc;

    NetworkInit(&network);
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            sleep(1);
            continue;
        }
        printf("Network connection successful!\n");
        break;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_client";
    while (1) {
        rc = MQTTConnect(&client, &data);
        if (rc != 0) {
            printf("MQTT connection failed: %d, retrying...\n", rc);
            sleep(2);
            continue;
        }
        printf("MQTT connected!\n");
        break;
    }

    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");

    while (1) {
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("MQTT publish failed: %d, retrying...\n", rc);
            MQTTDisconnect(&client);
            sleep(2);
            NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
            continue;
        }
        printf("MQTT published `work` to %s\n", MQTT_TOPIC);
        sleep(5);
    }

    NetworkDisconnect(&network);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}