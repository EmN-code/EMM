
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MESSAGE "work"

static sock_udp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    char msg[20];
    strncpy(msg, MESSAGE, sizeof(msg));
    msg[sizeof(msg) - 1] = '\0';

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    if (inet_pton(AF_INET, BROKER_ADDR, &broker.addr.ipv4) <= 0) {
        puts("Error parsing broker address");
        return 1;
    }

    if (emcute_con(&broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
