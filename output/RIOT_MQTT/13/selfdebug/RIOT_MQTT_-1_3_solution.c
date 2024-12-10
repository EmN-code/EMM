#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
#include "thread.h"
#include "timex.h"
#include "xtimer.h"

#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_riot_client"
#define TOPIC "device/state"
#define INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

// Network setup function - stub for illustration
void network_init(Network* n, const char* addr_str) {
    // Real network initialization code matching RIOT's or other library's API
}

int main(void) {
    Network network;
    unsigned char sendbuf[128], readbuf[128];

    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTString topicString = MQTTString_initializer;
    int rc;

    // Network Initialization
    printf("Setting up network...\n");
    network_init(&network, BROKER_IP);
    printf("Network ready.\n");

    // Initialize MQTT Client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the broker
    connectData.MQTTVersion = 3;
    connectData.clientID = MQTTString_initializer;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }
    printf("Connected to broker.\n");

    // Main loop
    while (1) {
        // Prepare the message
        char payload[20];
        snprintf(payload, sizeof(payload), "work");
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = payload;
        message.payloadlen = strlen(payload);

        topicString.cstring = TOPIC;
        rc = MQTTPublish(&client, topicString.cstring, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish, return code %d\n", rc);
        }
        else {
            printf("Published: %s\n", payload);
        }

        // Wait before next publish
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}