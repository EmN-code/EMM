#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <msg.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include "dht_params.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define SERVER_IP        "47.102.103.1"
#define SERVER_PORT      1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void *mqtt_client_thread(void *arg) {
    (void)arg;

    // Initialize MQTT Client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the network
    if (NetworkConnect(&network, SERVER_IP, SERVER_PORT) != 0) {
        printf("[MQTT] Network connection failed\n");
        return NULL;
    }

    // Prepare the MQTT message
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)"work or not work";
    message.payloadlen = strlen((char *)message.payload);

    while (1) {
        // Publish the message
        if (MQTTPublish(&client, "work/state", &message) != 0) {
            printf("[MQTT] Publish failed\n");
        } else {
            printf("[MQTT] Published: work or not work\n");
        }

        // Wait for next publish interval
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT client\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_client_thread, NULL, "mqtt_client_thread");

    // The main thread can handle other tasks or just idle
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}