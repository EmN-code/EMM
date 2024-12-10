#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "mutex.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "assert.h"
#include "net/gcoap.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP       "47.102.103.1"
#define PUB_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);
    return NULL; /* should never be reached */
}

static int publish_state(const char* state) {
    size_t len = strlen(state);
    int res = emcute_pub(&topic, state, len, EMCUTE_QOS_0);
    return res;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the broker\n");
        return 1;
    }

    /* initialize our subscription */
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        publish_state("work");
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    return 0;
}