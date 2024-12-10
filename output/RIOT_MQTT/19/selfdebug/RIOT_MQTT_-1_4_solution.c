#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define MQTT_CLIENT_ID "riot_mqtt_client"
#define MQTT_PUB_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void *mqtt_thread(void *arg) {
    (void)arg;

    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Network connect failed: %d\n", rc);
        return NULL;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("MQTT connect failed: %d\n", rc);
        return NULL;
    }

    while(1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = strcmp(MSG_WORK, "work") ? MSG_WORK : MSG_NOT_WORK;
        message.payloadlen = strlen((char *)message.payload);

        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
            printf("MQTT publish failed: %d\n", rc);
        }
        
        ztimer_sleep(ZTIMER_USEC, MQTT_PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}