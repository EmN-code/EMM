#include "arpa/inet.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "sys/socket.h"
#include "msg.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "stdlib.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "time_units.h"
#include "thread.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "netinet/in.h"
#include "evtimer_mbox.h"
#include "stddef.h"
#include "net/sock/udp.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_pub(void *arg) {
    (void)arg;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT-ESP32";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.id = 0;
        message.payload = (void *)MQTT_MESSAGE;
        message.payloadlen = strlen(MQTT_MESSAGE);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }

        xtimer_usleep(MQTT_INTERVAL);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, mqtt_pub, NULL, "mqtt_pub");
    return 0;
}