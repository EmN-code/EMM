#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "assert.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "string.h"
#include "arpa/inet.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define PUB_INTERVAL    (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Unable to connect to the gateway.");
        return NULL;
    }

    topic.name = "device/status";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Unable to register topic");
        return NULL;
    }

    while (1) {
        const char *message = "work or not work";

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Unable to publish message");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}