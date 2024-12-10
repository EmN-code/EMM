#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include "emcute.h"

#define EMCUTE_PORT     1883
#define SERVER_ADDR     "47.102.103.1"
#define DEFAULT_TOPIC   "state"
#define BUF_SIZE        64
#define PUB_INTERVAL    (5U * 1000000U)  // microseconds

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    char gw_addr_str[] = SERVER_ADDR;
    uint8_t buf[BUF_SIZE];

    // Convert IP address from string to network format
    if (inet_pton(AF_INET, gw_addr_str, &gw.addr.ipv4) != 1) {
        puts("ERROR: unable to parse IPv4 address");
        return 1;
    }

    // Initialize emcute client
    emcute_run(EMCUTE_PORT, "esp32_client");

    // Connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = DEFAULT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("ERROR: unable to register topic");
        return 1;
    }

    // Periodically publish the state
    while (1) {
        size_t len = snprintf((char*)buf, sizeof(buf), "work");  // Choose state "work"
        if (emcute_pub(&topic, buf, len, EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("ERROR: unable to publish data");
        }
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}