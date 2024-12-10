#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define EMCUTE_ID           "esp32"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883U)
#define EMCUTE_TOPIC_MAXLEN 64

static emcute_topic_t topic;
static char *state_msg = "work or not work";

static int publish_state(void) {
    unsigned flags = EMCUTE_QOS_0;
    topic.name = "device/state";

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&topic, state_msg, strlen(state_msg), flags) != EMCUTE_OK) {
        puts("Error: unable to publish state");
        return 1;
    }

    puts("Successfully published state");
    return 0;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) == EMCUTE_OK) {
        puts("Connected to MQTT-SN broker");
    } else {
        puts("Failed to connect to MQTT-SN broker");
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    emcute_discon();
    return 0;
}