#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUB_INTERVAL (5U * 1000U * 1000U) // 5 seconds

static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

void publish_state(void) {
    const char *state = "work or not work";
    emcute_reg(&topic);
    emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);
    gw.port = BROKER_PORT;

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Could not connect to gateway at address %s:%u
", BROKER_IP, BROKER_PORT);
        return 1;
    }

    topic.name = MQTT_TOPIC;

    while (1) {
        publish_state();
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}