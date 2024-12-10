#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "device/state"
#define STACK_SIZE 1024
#define Q_SZ 8

static char stack[STACK_SIZE];
static msg_t msg_queue[Q_SZ];

void *mqtt_pub_thread(void *arg) {
    (void)arg;

    msg_init_queue(msg_queue, Q_SZ);

    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    memset(&network, 0, sizeof(Network));
    
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = (char*)"mqtt_client";

    char address[100];
    sprintf(address, "%s:%d", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    int rc = NetworkConnect(&network, address, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker \n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen((char *)message.payload);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message \n");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_pub_thread, NULL, "mqtt_pub_thread");
    return 0;
}