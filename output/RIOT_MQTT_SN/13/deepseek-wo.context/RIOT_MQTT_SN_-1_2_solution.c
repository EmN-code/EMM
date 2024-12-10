
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MSG "work"

static sock_udp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    char msg[20];
    strcpy(msg, MSG);
    emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0);
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = { .family = AF_INET };

    // Initialize UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
