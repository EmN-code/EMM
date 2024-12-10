
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
static emcute_topic_t topic;

static void _register_topic(void)
{
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return;
    }
    printf("Registered topic: %s with ID: %u\n", topic.name, topic.id);
}

static void _publish_state(void)
{
    if (emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
        return;
    }
    puts("State published");
}

int main(void)
{
    // Initialize the network stack
    if (ipv4_addr_from_str("47.102.103.1", (ipv4_addr_t *)&gw.addr.ipv4) == NULL) {
        puts("Error: unable to parse broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    _register_topic();

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();
    puts("Disconnected from MQTT-SN broker");

    return 0;
}
