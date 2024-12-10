
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;

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
    // Initialize the network interface
    if (netif_set_hostname(netif_default, "riot_esp32") < 0) {
        puts("Error setting hostname");
        return 1;
    }

    // Set the gateway endpoint
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) <= 0) {
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

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
