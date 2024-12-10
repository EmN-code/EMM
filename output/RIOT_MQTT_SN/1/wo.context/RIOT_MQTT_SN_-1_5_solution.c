#include <stdio.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define EMCUTE_ID           "esp32_node"
#define EMCUTE_PORT         1883
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication.");
}

int connect_to_broker(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1") == NULL) {
        puts("Error: unable to parse IPv6 address." );
        return -1;
    }

    return emcute_con(&gw, true, NULL, NULL, 0, 0);
}

int publish_state(void) {
    const char *state_topic = "state/status";
    char *message = "working";
    topic.name = state_topic;
    emcute_reg(&topic);

    return emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
}

int main(void) {
    printf("MQTT-SN client starting\n");

    emcute_sub_t subscription;
    subscription.cb = on_pub;

    while (1) {
        if (connect_to_broker() == EMCUTE_OK) {
            printf("Connected to broker \n");
            publish_state();
        } else {
            puts("Connection failed, retrying in 5 seconds...");
        }
        xtimer_sleep(5);
    }

    return 0;
}