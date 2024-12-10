#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "MQTTClient.h"
#include "nimble/hci_common.h"
#include "nimble/nimble_npl.h"
#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "kernel.h"

#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "ESP32_Client"
#define TOPIC       "state"
#define QOS         1
#define TIMEOUT     10000L

void publish_state(MQTTClient client, const char* state) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    pubmsg.payload = (void*)state;
    pubmsg.payloadlen = (int)strlen(state);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    }
    else {
        printf("Waiting for publication of state on topic %s for client %s\n", TOPIC, CLIENTID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);
    }
}

int main(void) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    const char* state[] = {"work", "not work"};
    int idx = 0;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return EXIT_FAILURE;
    }

    while (1) {
        publish_state(client, state[idx]);
        idx = (idx + 1) % 2;
        sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return EXIT_SUCCESS;
}