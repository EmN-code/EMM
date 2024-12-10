#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define WAIT_INTERVAL (5U * US_PER_SEC)
#define STATE_TOPIC "device/state"

static sock_udp_ep_t broker_endpoint = {
    .family = AF_INET6,
    .port = BROKER_PORT
};

static mqtt_client_t client;
static mqtt_message_t message;

void mqtt_publish(mqtt_client_t* client, const char* topic, const char* payload) {
    mqtt_message_t msg;
    msg.payload = (void*)payload;
    msg.payloadlen = strlen(payload);
    msg.qos = 1;
    msg.retained = 0;
    mqtt_publish(client, topic, &msg);
    printf("Published %s on %s\n", payload, topic);
}

void* mqtt_handler(void *arg) {
    (void)arg;
    mqtt_connect(&client, BROKER_IP, BROKER_PORT);
    while (1) {
        mqtt_publish(&client, STATE_TOPIC, "work");
        xtimer_sleep(5);
        mqtt_publish(&client, STATE_TOPIC, "not work");
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    printf("Starting MQTT client\n");
    thread_create(mqtt_handler_stack, sizeof(mqtt_handler_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_handler, NULL, "mqtt_handler");
    return 0;
}
