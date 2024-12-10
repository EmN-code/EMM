#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include <stdio.h>
#include <string.h>
#include "emcute.h"
#include <msg.h>
#include <ztimer.h>

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     EMCUTE_PORT
#define TOPIC_NAME      "state"
#define MSG_TYPE        1

typedef struct {
    bool work_state; // true for "work", false for "not work"
} state_msg_t;

static emcute_topic_t state_topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static int publish_state(bool state)
{
    state_msg_t msg;
    msg.work_state = state;

    return emcute_pub(&state_topic, &msg, sizeof(msg), EMCUTE_QOS_0);
}

static void *mqtt_thread(void *arg)
{
    (void) arg;
    ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for safety before attempting connection

    sock_udp_ep_t gw = {.family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF};
    if (inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6) != 1) {
        printf("Error: unable to parse server address\n");
        return NULL;
    }
    gw.port = SERVER_PORT;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return NULL;
    }

    state_topic.name = TOPIC_NAME;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        if (publish_state(true) != EMCUTE_OK) {
            printf("Error: unable to publish state\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(stack, ARRAY_SIZE(stack));

    memset(&state_topic, 0, sizeof(state_topic));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");

    return 0;
}