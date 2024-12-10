#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <thread.h>
#include <ztimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * ZTIMER_MSEC_PER_SEC)
#define TOPIC_NAME "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    if (data != NULL) {
        printf("Received publication on topic %s: %.*s\n", topic->name, len, (char *)data);
    }
}

static void *publisher(void *arg) {
    sock_udp_ep_t gw = { .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Could not connect to [%s]:%d\n", BROKER_IP, (int) gw.port);
        return NULL;
    }
    
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Could not register topic %s\n", topic.name);
        return NULL;
    }
    
    while (1) {
        const char *state = "work";  // you can switch between "work" and "not work" for different states.
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published %s to topic %s\n", state, topic.name);
        }

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publisher, NULL, "publisher");
    return 0;
}