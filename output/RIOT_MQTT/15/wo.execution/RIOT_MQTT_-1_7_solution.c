#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "paho_mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "RIOT_ESP32"
#define MQTT_TOPIC "state"
#define SEND_INTERVAL_SEC (5U)

static sock_tcp_t tcp_sock;

void publish_state(sock_tcp_t *sock, const char *state) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    char address[100];

    snprintf(address, sizeof(address), "tcp://%s:%d", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    if (MQTTClient_create(&client, address, MQTT_CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS) {
        puts("Failed to create client");
        return;
    }

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        puts("Failed to connect");
        MQTTClient_destroy(&client);
        return;
    }

    pubmsg.payload = (void *)state;
    pubmsg.payloadlen = (int)strlen(state);
    pubmsg.qos = 1;
    pubmsg.retained = 0;

    if (MQTTClient_publishMessage(client, MQTT_TOPIC, &pubmsg, &token) != MQTTCLIENT_SUCCESS) {
        puts("Failed to publish");
    }

    MQTTClient_waitForCompletion(client, token, 1000L);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

int main(void) {
    puts("MQTT client application for RIOT on ESP32");

    while (1) {
        publish_state(&tcp_sock, "work");
        xtimer_sleep(SEND_INTERVAL_SEC);
        publish_state(&tcp_sock, "not work");
        xtimer_sleep(SEND_INTERVAL_SEC);
    }

    return 0;
}