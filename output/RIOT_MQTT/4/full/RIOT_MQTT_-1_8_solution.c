#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/paho_mqtt.h>
#include <MQTTClient.h>
#include <assert.h>
#include <arpa/inet.h>
//#include <mutex.h>
//#include <dht.h>
//#include <dht_params.h>
//#include <msg.h>
//#include <net/emcute.h>
//#include <net/gcoap.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_thread(void *arg) {
    (void)arg;

    char *topic = "test/topic";
    char *message = "work";
    unsigned char sendbuf[80];
    unsigned char readbuf[80];
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage mqtt_message;

    NetworkInit(&network);

    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Network connection failed\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.MQTTVersion = 4;
    connectData.clientID.cstring = "RIOT_ESP32";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("MQTT connection failed\n");
        return NULL;
    }

    while (1) {
        mqtt_message.qos = QOS0;
        mqtt_message.retained = 0;
        mqtt_message.payload = (void *)message;
        mqtt_message.payloadlen = strlen(message);

        if (MQTTPublish(&client, topic, &mqtt_message) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published\n");
        }

        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}