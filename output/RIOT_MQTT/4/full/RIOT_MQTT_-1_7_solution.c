#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stddef.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/paho_mqtt.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQ_CLIENT_BUFFER_SIZE 256
#define PUBLISH_INTERVAL 5  // seconds

static char stack[THREAD_STACKSIZE_MAIN];
static MQTTClient client;
static Network network;
char g_sendbuf[MQ_CLIENT_BUFFER_SIZE], g_readbuf[MQ_CLIENT_BUFFER_SIZE];

void publish_state(void) {
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    const char* state = "work";
    message.payload = (void*)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, "device/state", &message);
}

void *mqtt_thread(void *arg) {
    (void)arg;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot-client";

    while (1) {
        if(NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
            puts("ERROR: Unable to connect to network");
            ztimer_sleep(ZTIMER_MSEC, 5000);
            continue;
        }

        MQTTClientInit(&client, &network, 1000, g_sendbuf, MQ_CLIENT_BUFFER_SIZE, g_readbuf, MQ_CLIENT_BUFFER_SIZE);
        if(MQTTConnect(&client, &connectData) != 0) {
            puts("ERROR: Unable to connect to MQTT broker");
            ztimer_sleep(ZTIMER_MSEC, 5000);
            continue;
        }

        while (1) {
            publish_state();
            ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
        }
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}
