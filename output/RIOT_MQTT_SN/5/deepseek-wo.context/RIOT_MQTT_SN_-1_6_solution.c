
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/emcute.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static emcute_topic_t topic;

static void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    }
}

int main(void)
{
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = EMCUTE_PORT;

    if (sock_udp_ep_from_str(&gw, EMCUTE_ADDR) < 0) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        emcute_discon();
        return 1;
    }

    puts("Topic registered");

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}
