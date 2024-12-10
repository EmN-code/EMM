
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;

static void _publish_state(void)
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
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

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
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
