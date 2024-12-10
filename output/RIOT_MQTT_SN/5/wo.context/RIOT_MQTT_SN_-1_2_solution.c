#include "xtimer.h"
#include "net/emcute.h"
#include "net/netif.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_device"
#define TOPIC_NAME "device/state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;

static void *pub_state(void *arg) {
    (void)arg;
    uint8_t state = 0;

    while (1) {
        const char *state_str = state ? "work" : "not work";
        int res = emcute_pub(&topic, state_str, strlen(state_str), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish data (Error: %d)\n", res);
        } else {
            printf("Published: %s\n", state_str);
        }
        state = !state;  // Toggle state
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    printf("Connecting to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    sock_udp_ep_t gw = {.family = AF_INET6, .port = BROKER_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker\n");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic\n");
        return 1;
    }

    pub_state(NULL);
    return 0;
}