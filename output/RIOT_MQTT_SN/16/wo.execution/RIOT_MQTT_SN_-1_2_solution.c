#include "thread.h"
#include "event/thread.h"
#include "net/gcoap.h"
#include "net/emcute.h"

#define BROKER_PORT     1883
#define BROKER_ADDRESS  "47.102.103.1"
#define PUBLISH_INTERVAL (5U * 1000000U) /* in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *publisher(void *arg) {
    (void)arg;

    emcute_topic_t topic;
    unsigned flags = EMCUTE_QOS_0; // using QoS level 0
    char *message = "work or not work";

    /* register topic */
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return NULL;
    }

    topic.id = 0;  // assuming the first topic id assigned is 0

    while (1) {
        if (emcute_pub(&topic, message, strlen(message), flags) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .addr = { .ipv6 = { 0 } }, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)gw.addr.ipv6, BROKER_ADDRESS);

    /* connect to gateway */
    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        xtimer_sleep(1);
    }

    /* start publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");

    return 0;
}