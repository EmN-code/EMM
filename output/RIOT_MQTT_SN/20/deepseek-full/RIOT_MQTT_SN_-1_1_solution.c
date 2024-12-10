
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define MSG_SIZE 128

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    char msg[MSG_SIZE];
    snprintf(msg, sizeof(msg), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0);
}

void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse gateway address");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    puts("Connected to MQTT-SN broker");

    if (emcute_sub(&topic, TOPIC, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return NULL;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("Starting MQTT-SN client");

    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
