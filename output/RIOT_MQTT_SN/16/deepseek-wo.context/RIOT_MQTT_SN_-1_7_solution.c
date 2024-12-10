
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_sub_t sub;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

void on_pub_msg(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Received message on topic %s\n", topic->name);
}

int main(void) {
    // Initialize the network stack
    if (netif_set_opt(netif_default, NETOPT_IPV4_ADDR, 0, &ipv4_addr_unspecified, sizeof(ipv4_addr_t)) < 0) {
        puts("Error setting default interface IP address");
        return 1;
    }

    // Convert server address from string to binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error converting server address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Subscribe to the topic
    sub.topic = topic;
    sub.cb = on_pub_msg;
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }

    // Publish state every 5 seconds
    char state[] = "work";
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", state);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
