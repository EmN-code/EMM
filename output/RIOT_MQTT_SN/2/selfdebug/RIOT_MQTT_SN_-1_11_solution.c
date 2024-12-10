#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#include "net/sock/udp.h"
#include "net/emcute.h"
#include "kernel.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
#define EMCUTE_PORT         (1883)
#define SERVER_ADDR         "47.102.103.1"

static emcute_topic_t _state_topic;
static char *_state_topic_name = "device/state";

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Received some data\n");
}

static void *pub_thread(void *arg) {
    (void)arg;
    char *states[] = { "work", "not work" };
    size_t index = 0;
    while (1) {
        char *state = states[index];
        unsigned flags = EMCUTE_QOS_0;
        if (emcute_pub(&_state_topic, state, strlen(state), flags) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published: %s\n", state);
        }
        index = (index + 1) % (sizeof(states)/sizeof(states[0]));
        sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* configure the MQTT-SN broker address and port */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR);

    /* initialize emcute */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    }

    /* setup subscription */
    memset(&_state_topic, 0, sizeof(_state_topic));
    _state_topic.name = _state_topic_name;
    if (emcute_reg(&_state_topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID\n");
        return 1;
    }

    /* start thread to publish data */
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "pub_thread");

    /* run the emcute receiver loop */
    emcute_run(EMCUTE_PORT, "mqttsn-client");

    return 0;
}