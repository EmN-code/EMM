#include "net/sock/udp.h"
#include "net/mqttsn.h"
#include "xtimer.h"
#include "thread.h"
#include <stdio.h>

#ifndef SERVER
#define SERVER "47.102.103.1"
#endif
#define PORT 1883
#define INTERVAL (5U * US_PER_SEC)

static mqttsn_client_t mqtt_client;
static char stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_thread_handler(void *arg) {
    (void)arg;
    uint32_t message_counter = 0;
    char *topic = "state";

    while (1) {
        char payload[64];
        snprintf(payload, sizeof(payload), "work %s", (message_counter % 2 == 0) ? "true" : "false");
        mqttsn_send_publish(&mqtt_client, topic, payload, strlen(payload), MQTTSN_QOS_0, false);
        printf("Published state: %s\n", payload);

        xtimer_sleep(INTERVAL / US_PER_SEC);
        message_counter++;
    }

    return NULL;
}

int main(void) {
    mqttsn_create_socket(&mqtt_client.net.sock, SERVER, PORT);

    mqttsn_init(&mqtt_client, &mqtt_client.net.sock);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread_handler, NULL, "MQTT thread");

    while (1) {
        thread_yield();
    }

    return 0;
}