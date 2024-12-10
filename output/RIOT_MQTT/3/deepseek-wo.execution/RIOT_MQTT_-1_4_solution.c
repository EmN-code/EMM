
#include <net/sock.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <stddef.h>
#include <net/sock/udp.h>

#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "riot_esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

int main(void)
{
    MQTTClient client;
    Network network;
    int rc;

    // Initialize the network
    NetworkInit(&network);

    // Create MQTT client
    MQTTClient_create(&client, &network, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // Set connection options
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connect to the MQTT broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    // Publish state every 5 seconds
    while (1) {
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = (void*)PAYLOAD;
        pubmsg.payloadlen = strlen(PAYLOAD);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_deliveryToken token;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        MQTTClient_waitForCompletion(client, token, TIMEOUT);

        printf("Message '%s' with delivery token %d delivered\n", PAYLOAD, token);

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    // Disconnect and destroy the client
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return EXIT_SUCCESS;
}
