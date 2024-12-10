#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "net/sock/udp.h"  // Added header for socket operations
#include "net/emcute.h"  // Include emcute library for MQTT-SN
#include "xtimer.h"  // Include xtimer for timers in RIOT OS

#define EMCUTE_ID           "mqtt_client"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define EMCUTE_PORT         SERVER_PORT
#define TOPIC_NAME          "state"
#define SEND_INTERVAL       (5U * US_PER_SEC)  // 5 seconds interval

static sock_udp_ep_t gw = {
    .family = AF_INET,  // Internet address family
    .port = EMCUTE_PORT  // MQTT-SN server port
};

static emcute_topic_t topic;

void publish_state(const char* state) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("[Error] Unable to publish data\n");
    }
    else {
        printf("Published state: %s\n", state);
    }
}

void *pub_thread(void *arg) {
    (void)arg;
    char *state = "work";
    while (1) {
        publish_state(state);
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
        state = strcmp(state, "work") == 0 ? "not work" : "work";
    }
    return NULL;
}

int main(void) {
    // Setup network address
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        puts("Error: invalid address\n");
        return 1;
    }

    // Connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT broker\n");
        return 1;
    }

    // Setup topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID\n");
        return 1;
    }

    // Start publisher thread
    thread_create(stack_pub, sizeof(stack_pub), EMCUTE_PRIO, 0,
                  pub_thread, NULL, "pub_thread");

    return 0;
}