#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "net/gcoap.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID   "riot_client"
#define MQTT_TOPIC       "state"
#define PAYLOAD_ON       "work"
#define PAYLOAD_OFF      "not work"
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

static char sendbuf[128];
static char readbuf[128];

void* publish_thread(void *arg) {
    (void)arg;
    Network network;
    MQTTClient client;
    MQTTClientInit(&client, &network, 10000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        puts("Failed to connect to broker");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = 1;
        message.retained = 0;
        message.payload = (unsigned char *)PAYLOAD_ON;
        message.payloadlen = strlen(PAYLOAD_ON);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            puts("Error while publishing");
        } else {
            puts("Message published");
        }

        xtimer_usleep(MESSAGE_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");
    return 0;
}