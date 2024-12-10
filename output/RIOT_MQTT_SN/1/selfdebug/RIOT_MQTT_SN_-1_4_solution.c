#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <net/ipv6/addr.h> // Contains inet_pton declaration.
#include <net/emcute.h>

#define EMCUTE_PORT          (1883U)
#define MQTT_SN_BROKER_IP    "47.102.103.1"
#define PUBLISH_INTERVAL     (5U) // 5 seconds
#define XTIMER_SLEEP         (5U * US_PER_SEC) // Sleep time in microseconds

static emcute_topic_t topic;
static char *topic_name = "state";

void publish_state(void) {
    const char *state_msg = "work";
    size_t len = strlen(state_msg);

    if (emcute_pub(&topic, state_msg, len, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish data");
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, MQTT_SN_BROKER_IP, &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}