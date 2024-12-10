#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define PUB_TOPIC 0
#define PUB_INTERVAL (5U)

static emcute_topic_t _topic;

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

    // converting IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse server address");
        return 1;
    }
    
    // connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker");
        return 1;
    }
    puts("Connected to broker");

    // set up the topic we want to publish to
    _topic.name = "state";
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        // Publish 'work'
        if (emcute_pub(&_topic, "work", strlen("work"), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Published: work");
        }

        // Sleep for PUB_INTERVAL seconds
        xtimer_sleep(PUB_INTERVAL);
    }
    return 0;
}