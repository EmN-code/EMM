#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MQTTClient.h"  // Ensure this is the last header as per requirements

#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "ESP32Client"
#define TOPIC       "state"
#define PAYLOAD1    "work"
#define PAYLOAD2    "not work"
#define QOS         1
#define TIMEOUT     10000L

MQTTClient client;

void publish_state(const char *state) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (char *)state;
    pubmsg.payloadlen = (int)strlen(state);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    int rc;

    if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    }

    printf("Waiting for publication of %s on topic %s for client with ClientID: %s\n",
           state, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
}

int main(void) {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while (1) {
        publish_state(PAYLOAD1);
        sleep(5);
        publish_state(PAYLOAD2);
        sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}