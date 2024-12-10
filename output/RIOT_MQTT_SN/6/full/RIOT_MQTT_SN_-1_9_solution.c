#include "ztimer.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define BROKER_PORT         (1883U)
#define PUB_INTERVAL        (5U) // Publish interval in seconds

static emcute_sub_t subscriptions[NUMOFSUBS];
static char stack[THREAD_STACKSIZE_DEFAULT];

static unsigned flags = EMCUTE_QOS_0;
static char * mqtt_topics[] = { "status" };

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(BROKER_PORT, "local");
    return NULL; // This will never be reached
}

static int publish(const char *topic, const char *data) {
    emcute_topic_t t;
    unsigned len;

    len = strlen(data);

    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("[FAILED] Unable to register topic\n");
        return 1;
    }

    if (emcute_pub(&t, data, len, flags) != EMCUTE_OK) {
        printf("[FAILED] Unable to publish data\n");
        return 1;
    }

    printf("Published %i bytes to topic '%s'\n", (int)len, topic);
    return 0;
}

int main(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = BROKER_PORT
    };

    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    while (1) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("Connection failed, retrying...\n");
            continue;
        }

        publish(mqtt_topics[0], "working");
        xtimer_sleep(PUB_INTERVAL);
    }
    return 0;
}