
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_sub_t sub;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    printf("Received message on topic %s\n", topic->name);
}

int main(void)
{
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("No default network device found");
        return 1;
    }

    // Initialize the EMCUTE stack
    emcute_init();

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }
    puts("Connected to MQTT-SN gateway");

    // Register the topic
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Subscribe to the topic
    sub.topic = topic;
    sub.cb = _on_pub;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }

    // Publish state every 5 seconds
    char state[] = "work";
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", state);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
