#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <emcute.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5 * 1000000U)  // 5 seconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // Handle received publish, if any
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t*)&gw.addr, SERVER_ADDR);

    emcute_run(SERVER_PORT, "device_id");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to the broker.\n");
        return 1;
    }

    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic.\n");
        return 1;
    }

    while (1) {
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish data.\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}