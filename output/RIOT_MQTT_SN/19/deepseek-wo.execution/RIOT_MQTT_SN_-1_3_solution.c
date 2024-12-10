
#include <net/ipv6/addr.h>
#include <thread.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static emcute_topic_t topic;

void *publish_state(void *arg) {
    const char *state = "work";
    while (1) {
        int res = emcute_pub(&topic, state, strlen(state), 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = "state";
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    thread_create(stack, sizeof(stack), PRIORITY, 0, publish_state, NULL, "publisher");

    return 0;
}
