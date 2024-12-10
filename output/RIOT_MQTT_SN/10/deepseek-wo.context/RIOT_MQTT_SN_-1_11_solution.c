
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Run the emcute thread
    emcute_run(SERVER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
