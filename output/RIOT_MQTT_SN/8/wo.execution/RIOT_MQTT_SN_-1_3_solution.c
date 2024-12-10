#include "net/sock/util.h"
#include "thread.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "stdio.h"
#include "event/thread.h"
#include "net/sock/udp.h"
#include "mma8x5x_regs.h"
#include "stdlib.h"
#include "mma8x5x_params.h"
#include "net/gcoap.h"

#define EMCUTE_ID "device_id"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define STATE_PUB_INTERVAL (5U)
#define MQTT_SN_PORT (1883)
#define SERVER_ADDR "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static const char *state = "work or not work";
static emcute_topic_t topic;
static sock_udp_ep_t gw = {.family = AF_INET, .port = MQTT_SN_PORT};
static char *subscriptions[1];

static void *thread_func(void *arg) {
    (void) arg;
    while (1) {
        emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        ztimer_sleep(ZTIMER_MSEC, STATE_PUB_INTERVAL * 1000);
    }
    return NULL;
}

int main(void) {
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);
    subscriptions[0] = NULL;

    emcute_run(EMCUTE_PRIO, stack, sizeof(stack), EMCUTE_ID);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    }

    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  thread_func, NULL, "pub_thread");

    return 0;
}