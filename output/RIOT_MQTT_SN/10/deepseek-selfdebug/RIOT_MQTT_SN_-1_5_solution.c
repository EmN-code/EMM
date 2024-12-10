
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char mqtt_sn_broker_addr_str[] = MQTT_SN_BROKER_IP;
static sock_udp_ep_t mqtt_sn_broker = SOCK_IPV4_EP_ANY;

static emcute_topic_t topic;

static void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static unsigned state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % (sizeof(states) / sizeof(states[0]));

    printf("Publishing state: %s\n", state);

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    }
}

int main(void)
{
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("Error: no network devices available");
        return 1;
    }

    // Initialize the MQTT-SN broker endpoint
    mqtt_sn_broker.port = MQTT_SN_BROKER_PORT;
    if (inet_pton(AF_INET, mqtt_sn_broker_addr_str, &mqtt_sn_broker.addr.ipv4) <= 0) {
        puts("Error: invalid MQTT-SN broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&mqtt_sn_broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
