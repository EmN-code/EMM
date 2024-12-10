#include "net/gnrc.h"
#include "net/emcute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATUS_TOPIC_NAME "device/status"
#define STATUS_WORKING "work"
#define STATUS_NOT_WORKING "not work"
#define PUBLISH_INTERVAL (5U)

static emcute_topic_t status_topic;

static void publish_status(const char *status) {
    emcute_pub(&status_topic, status, strlen(status), EMCUTE_QOS_0);
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET, .port = MQTT_BROKER_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_ADDR);

    puts("Connecting to MQTT broker...");
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", MQTT_BROKER_ADDR, MQTT_BROKER_PORT);
        return 1;
    }

    status_topic.name = STATUS_TOPIC_NAME;
    emcute_reg(&status_topic);

    while (1) {
        publish_status(STATUS_WORKING);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}