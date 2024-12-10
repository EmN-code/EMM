
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);

    int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    }
}

int main(void)
{
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

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

    // Run the emcute loop
    emcute_run(EMQX_SERVER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
