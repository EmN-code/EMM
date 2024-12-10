#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h> // Ensure "net/ipv6/addr.h" is included for network types
#include "paho-mqtt/MQTTClient-C/src/MQTTClient.h"

// Define constants for the connection
#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "ESP32Client"
#define TOPIC       "state"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L

int main(void) {
    int rc;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTPacket_connectData_initializer;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(&client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while(1) {
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        MQTTClient_deliveryToken token;
        pubmsg.payload = PAYLOAD;
        pubmsg.payloadlen = (int)strlen(PAYLOAD);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);

        sleep(5); // Publish every 5 seconds
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}