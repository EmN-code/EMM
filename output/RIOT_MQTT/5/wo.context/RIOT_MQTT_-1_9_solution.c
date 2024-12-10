#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define QOS         1
#define TIMEOUT     10000L

mqtt_client_t client;
sock_udp_ep_t gw = {.family = AF_INET6, .port = 1883};

static int publish_state(const char *state) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    pubmsg.payload = (void *)state;
    pubmsg.payloadlen = strlen(state);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(client, "device/state", &pubmsg, &token);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    return rc;
}

int main(void) {
    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, "tcp://47.102.103.1:1883", "ESP32Client",
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    while (1) {
        publish_state("work");
        sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}