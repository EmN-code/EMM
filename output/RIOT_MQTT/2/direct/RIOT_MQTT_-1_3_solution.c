#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mqtt.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include "thread.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define CLIENT_ID "esp32_client"
#define TOPIC "esp32/state"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_con_t connection;

static void *mqtt_thread(void *arg) {
    (void)arg;
    xtimer_ticks32_t last_pub = xtimer_now();

    while (1) {
        char *state = "work";
        emcute_pub_t pub;
        memset(&pub, 0, sizeof(pub));
        pub.topic.name = TOPIC;
        pub.data = (void*)state;
        pub.data_len = strlen(state);
        pub.qos = EMCUTE_QOS_0;

        if (emcute_pub(&pub) != EMCUTE_OK) {
            printf("[MQTT] Error: Unable to publish\n");
        } else {
            printf("[MQTT] Published: %s\n", state);
        }

        xtimer_periodic_wake(&last_pub, PUB_INTERVAL);
        state = strcmp(state, "work") == 0 ? "not work" : "work";
    }

    return NULL;
}

static int connect_to_broker(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };

    if (ipv6_addr_from_str((ipv6_addr_t *) &gw.addr.ipv6, BROKER_IP) == NULL) {
        printf("[MQTT] Error: Unable to parse Broker IP address\n");
        return -1;
    }

    if (emcute_con(&connection, gw, 1, CLIENT_ID, NULL) != EMCUTE_OK) {
        printf("[MQTT] Error: Unable to connect to Broker\n");
        return -1;
    }

    printf("[MQTT] Connected to Broker\n");
    return 0;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    if (connect_to_broker() != 0) {
        printf("[MQTT] Connection to Broker failed\n");
        return -1;
    }

    // Main thread can perform other tasks or just idle
    while (1) {
        // Keep running indefinitely
    }

    return 0;
}