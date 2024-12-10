#include <thread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "periph/uart.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "msg.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U) /** Interval in seconds */
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];
static msg_t queue[8];

static void *publisher(void *arg) {
    (void)arg;
    char *state = "work";
    int rc;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    char address[50];
    snprintf(address, sizeof(address), "tcp://%s:%d", BROKER_IP, BROKER_PORT);

    MQTTClient_create(&client, address, "ESP32_client",
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while (1) {
        pubmsg.payload = state;
        pubmsg.payloadlen = strlen(state);
        pubmsg.qos = 1;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(client, "state", &pubmsg, &token);
        printf("Waiting for publication of %s on topic state\n", state);
        rc = MQTTClient_waitForCompletion(client, token, 1000L);
        printf("Message with delivery token %d delivered\n", token);

        sleep(PUB_INTERVAL);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");
    return 0;
}