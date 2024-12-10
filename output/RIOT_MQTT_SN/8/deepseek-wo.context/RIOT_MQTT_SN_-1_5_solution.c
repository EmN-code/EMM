
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"
#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static sock_udp_ep_t gw = { .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void _publish_state(void)
{
    char msg[128];
    snprintf(msg, sizeof(msg), "%s", STATE_MESSAGE);

    if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Published state message");
    }
}

int main(void)
{
    // Initialize the network stack
    if (netif_set_default(netif_get_by_name("esp_wifi0")) < 0) {
        puts("Error: unable to set default network interface");
        return 1;
    }

    // Set the gateway endpoint
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error: unable to parse gateway IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    printf("Registered topic: %s with ID: %u\n", topic.name, topic.id);

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
