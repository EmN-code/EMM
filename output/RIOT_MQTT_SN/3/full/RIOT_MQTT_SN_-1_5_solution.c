#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "timex.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stddef.h"
#include "string.h"
#include "assert.h"
#include "arpa/inet.h"
#include "msg.h"
#include "mutex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "dht.h"
#include "dht_params.h"
#include "mqttclient.h"
#include "paho_mqtt.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "time_units.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topstack[THREAD_STACKSIZE_DEFAULT];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    (void)topic;
    printf("Received publication: Topic ID - %i, Data - %s\n", topic->id, in);
}

int main(void) {
    /* Start the emcute thread */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = {.family = AF_INET, .port = 1883};
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    emcute_topic_t t;
    char *topicname = "state";
    t.name = topicname;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return 1;
    }

    char *message = "work or not work";
    while (1) {
        if (emcute_pub(&t, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        xtimer_sleep(5); /* Sleep for 5 seconds */
    }

    return 0;
}