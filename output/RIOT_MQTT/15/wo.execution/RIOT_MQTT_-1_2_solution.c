#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <thread.h>
#include <event/thread.h>
#include "MQTTClient.h"

#define MQTT_BROKER_ADDRESS "tcp://47.102.103.1:1883"
#define CLIENT_ID           "esp32_client"
#define TOPIC               "device/state"
#define QOS                 1
#define TIMEOUT             10000L

void publish_state(MQTTClient client, const char *state) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    pubmsg.payload = (void *)state;
    pubmsg.payloadlen = strlen(state);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    }
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, MQTT_BROKER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    const char *state_work = "work";
    const char *state_not_work = "not work";

    while (1) {
        publish_state(client, state_work);
        sleep(5);
        publish_state(client, state_not_work);
        sleep(5);
    }

    MQTTClient_disconnect(client, 10000L);
    MQTTClient_destroy(&client);
    return 0;
}