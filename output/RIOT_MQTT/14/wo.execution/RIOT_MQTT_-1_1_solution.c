#include <string.h>
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "net/conn/ip.h"
#include "net/sock/udp.h"
#include "mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "device/state"
#define PUB_MSG "work"
#define NOT_WORK_MSG "not work"

static char stack[THREAD_STACKSIZE_MAIN];

static void *publish_state(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET6,
                         .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr);
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "mqtt_client";

    if (MQTTConnect(&client, &data) != 0) {
        printf("Failed to connect MQTT client\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = PUB_MSG; // You can toggle between PUB_MSG or NOT_WORK_MSG as needed
        message.payloadlen = strlen(PUB_MSG);

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published: %s\n", (char*)message.payload);
        }

        xtimer_sleep(5);  // Sleep for 5 seconds before the next publish
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    return NULL;
}

int main(void) {
    printf("Starting MQTT publisher\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publish_state, NULL, "publish_thread");
    return 0;
}