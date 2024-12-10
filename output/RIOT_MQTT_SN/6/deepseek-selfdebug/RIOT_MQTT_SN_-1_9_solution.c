
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT 1883
#define EMCUTE_TOPIC "state"
#define EMCUTE_MSG "work"

static emcute_sub_t sub;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic; (void)data; (void)len;
    printf("Received message on topic: %s\n", topic->name);
}

int main(void)
{
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    // Initialize UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert IP address string to binary form
    const char *broker_ip = "47.102.103.1";
    if (inet_pton(AF_INET, broker_ip, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    // Register topic
    emcute_topic_t topic;
    topic.name = EMCUTE_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Subscribe to the topic
    sub.topic = topic;
    sub.cb = on_pub;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, EMCUTE_MSG, strlen(EMCUTE_MSG), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("State published");
        }
        xtimer_sleep(5);
    }

    // Disconnect from the broker
    emcute_discon();

    return 0;
}
