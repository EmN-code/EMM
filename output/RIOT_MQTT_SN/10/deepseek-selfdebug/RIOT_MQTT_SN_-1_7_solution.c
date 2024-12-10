
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PORT 1883
#define BROKER_ADDR "47.102.103.1"
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"

static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    uint8_t addr[sizeof(struct in_addr)];

    if (inet_pton(AF_INET, BROKER_ADDR, addr) <= 0) {
        puts("Error: invalid broker address");
        return 1;
    }

    gw.family = AF_INET;
    gw.netif = SOCK_ADDR_ANY_NETIF;
    memcpy(&gw.addr.ipv4, addr, sizeof(gw.addr.ipv4));

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    puts("Topic registered");

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
