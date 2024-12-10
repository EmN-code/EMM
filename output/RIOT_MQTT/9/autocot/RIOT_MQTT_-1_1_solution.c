#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <timex.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/mqttsn/mqttsn.h>

#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     1883
#define MQTT_TOPIC      "state"
#define MQTT_MESSAGE    "work"
#define INTERVAL        (5 * US_PER_SEC)  // 5 seconds

static emcute_topic_t topic;

void publish_state(void) {
    while (1) {
        emcute_reg(&topic);
        emcute_pub(&topic, MQTT_MESSAGE, strlen(MQTT_MESSAGE));
        xtimer_sleep(INTERVAL);
    }
}

int main(void) {
    /* Connect to MQTT broker */
    sockaddr_in6 server_addr = {0};
    ipv6_addr_from_str(&server_addr.sin6_addr, SERVER_ADDR);
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(SERVER_PORT);

    emcute_con(server_addr);

    /* Initialize topic */
    topic.name = MQTT_TOPIC;
    topic.id = 0; // Broker assigned

    /* Publish state every five seconds */
    publish_state();

    return 0;
}