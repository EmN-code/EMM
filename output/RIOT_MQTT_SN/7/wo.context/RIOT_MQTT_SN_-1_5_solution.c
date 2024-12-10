#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT        (1883U)
#define SERVER_ADDR        "47.102.103.1"
#define PUB_TOPIC          "state"
#define PUB_INTERVAL       (5U)

static emcute_topic_t topic;
static char client_id[64];

static int publish_state(void) {
    const char *state = "work or not work";
    size_t len = strlen(state);
    
    return emcute_pub(&topic, state, len, EMCUTE_QOS_0);
}

static void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = {.family = AF_INET6, .port = EMCUTE_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, SERVER_ADDR);
    
    thread_getpid()], (thread_status_t[]){HTTP_WAIT});
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to broker at %s\n", SERVER_ADDR);
    }
    
    topic.name = PUB_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
    }

    while (1) {
        if (publish_state() != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    snprintf(client_id, sizeof(client_id), "client_%u", (unsigned)thread_getpid());
    
    static char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), EMCUTE_RUN_PRIO, 0, mqtt_thread, NULL, "mqtt_thread");

    emcute_run(EMCUTE_PORT, client_id);
    return 0;
}