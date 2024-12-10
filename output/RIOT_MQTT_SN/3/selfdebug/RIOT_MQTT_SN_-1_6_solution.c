#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include "emcute.h"

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDRESS  "47.102.103.1"
#define PUBLISH_INTERVAL (5U)

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    char *app_state = "work";
    msg_init_queue(main_msg_queue, sizeof(main_msg_queue) / sizeof(main_msg_queue[0]));

    if (ipv6_addr_from_str((ipv6_addr_t *)gw.addr.ipv6, SERVER_ADDRESS) == NULL) {
        printf("Error: unable to parse gateway address\n");
        return 1;
    }

    emcute_run(EMCUTE_PORT, "RIOT_app");

    topic.name = "app/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, app_state, strlen(app_state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}