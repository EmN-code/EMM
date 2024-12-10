#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "thread.h"
#include "MQTTClient.h"

#define EMCUTE_PORT     (1883U)
#define MESSAGE_INTERVAL (5U)
#define MQTT_CLIENT_ID  "esp32_client"
#define MESSAGE_PAYLOAD "state: work"
#define SERVER_ADDR     "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    int rc;

    // Initialize the network structure
    NetworkInit(&network);
    rc = NetworkConnect(&network, SERVER_ADDR, EMCUTE_PORT);
    if (rc != SUCCESS) {
        puts("Error: Unable to connect MQTT network");
        return NULL;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, stack, sizeof(stack), stack, sizeof(stack));

    // Setup the connection data
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connect_data);
    if (rc != SUCCESS) {
        puts("Error: Unable to connect MQTT client");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = MESSAGE_PAYLOAD;
        message.payloadlen = strlen(MESSAGE_PAYLOAD);

        rc = MQTTPublish(&client, "iot/status", &message);
        if (rc != SUCCESS) {
            puts("Error: Unable to publish message");
        }

        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL,
                  "mqtt_publisher");

    return 0;
}