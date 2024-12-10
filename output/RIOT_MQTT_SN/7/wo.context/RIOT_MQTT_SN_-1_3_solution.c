#include <xtimer.h> // for delay and timer functions
#include <stdio.h> // for standard input and output functions
#include <string.h> // for string handling functions
#include "net/emcute.h" // include the emcute header file for MQTT-SN functionalities
#include "net/ipv6/addr.h" // for handling IPv6 addresses

#define BROKER_IP "47.102.103.1" // define the broker IP
#define BROKER_PORT 1883 // define broker port
#define SLEEP_INTERVAL 5 // sleep interval in seconds
#define TOPIC_NAME "device/status" // topic name to publish to

static emcute_topic_t topic;

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    puts("Connecting to MQTT-SN broker...");
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker.");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic.");
        return 1;
    }

    const char *messages[2] = {"work", "not work"};
    size_t i = 0;

    while (1) {
        if (emcute_pub(&topic, messages[i], strlen(messages[i]), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data.");
        }
        i = (i + 1) % 2; // toggle between 0 and 1
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}
