
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t broker = { .family = AF_INET, .port = MQTT_BROKER_PORT };
static emcute_topic_t topic;

static void _publish_state(void)
{
    int res;
    unsigned flags = EMCUTE_QOS_0;

    res = emcute_pub(&topic, MQTT_MESSAGE, strlen(MQTT_MESSAGE), flags);
    if (res != EMCUTE_OK) {
        printf("Error on publish: %d\n", res);
    } else {
        printf("Published '%s' to topic '%s'\n", MQTT_MESSAGE, topic.name);
    }
}

int main(void)
{
    sock_udp_t sock;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the UDP socket
    sock_udp_create(&sock, NULL, NULL, 0);

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4) <= 0) {
        printf("Error converting IP address\n");
        return 1;
    }

    // Initialize the emcute thread
    emcute_run(MQTT_BROKER_PORT, MQTT_CLIENT_ID);

    // Register the topic
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error registering topic\n");
        return 1;
    }

    while (1) {
        // Publish the state every 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
        _publish_state();
    }

    return 0;
}
