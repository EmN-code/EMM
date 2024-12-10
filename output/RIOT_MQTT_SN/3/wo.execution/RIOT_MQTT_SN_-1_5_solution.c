#include <ztimer.h>
#include <thread.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define EMCUTE_ID_SIZE (8)
#define EMCUTE_PRIO    (THREAD_PRIORITY_MAIN - 1)
#define PUBLISH_INTERVAL (5U)
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT (1883)
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"
#define TOPIC_NAME "system/state"

static char stack[THREAD_STACKSIZE_MAIN];
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t topic;

void *publish_state(void *arg) {
    (void)arg;
    unsigned short state_toggle = 0;
    
    while (1) {
        const char *data = state_toggle ? STATE_WORK : STATE_NOT_WORK;
        if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        }
        state_toggle = !state_toggle;
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * MS_PER_SEC);
    }
    return NULL;
}

int main(void) {
    msg_init_queue();
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }
    
    strcpy(topic.name, TOPIC_NAME);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }
    
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publish_state, NULL, "pub_state");
    
    emcute_run(SERVER_PORT, "esp32client");
    return 0;
}