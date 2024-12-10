#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <timex.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <paho_mqtt.h>
#include <thread.h>
#include <xtimer.h>
#include <ztimer.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL     (5U * US_PER_SEC) // 5 seconds

static char mqtt_client_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

void *mqtt_thread(void *arg) {
    (void) arg;

    MQTTClient client;
    Network network;
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, malloc(256), 256, malloc(256), 256);

    char addr[100];
    snprintf(addr, 100, "%s:%d", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("NetworkConnect failed: %d\n", rc);
        return NULL;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTTConnect failed: %d\n", rc);
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS1;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        rc = MQTTPublish(&client, "status", &message);
        if (rc != 0) {
            printf("MQTTPublish failed: %d\n", rc);
        }

        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue)/sizeof(msg_t));

    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_client");

    return 0;
}