#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_PUB_TOPIC "device/state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkConnect(&network, BROKER_ADDRESS, BROKER_PORT);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) == SUCCESS) {
        MQTTClient_publishMessage(&client, MQTT_PUB_TOPIC, &message, NULL);
        printf("Published state: %s\n", MQTT_MESSAGE);
    } else {
        printf("Failed to connect to MQTT broker\n");
    }

    MQTTClient_disconnect(&client);
    NetworkDisconnect(&network);
}

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_handler, NULL, "mqtt_thread");
    return 0;
}