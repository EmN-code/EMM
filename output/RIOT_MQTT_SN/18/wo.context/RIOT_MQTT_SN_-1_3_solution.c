#include "emcute.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         1883
#define MQTT_BROKER_IP      "47.102.103.1"
#define TOPIC_NAME          "status"
#define PUBLISH_INTERVAL    (5U * US_PER_SEC)

static emcute_topic_t topic;
static char *status = "work";

void publish_status(void) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&topic, status, strlen(status), flags) != EMCUTE_OK) {
        puts("error: unable to publish status");
    }
}

int main(void) {
    printf("Setup MQTT-SN client\n");

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    char *topic_name = TOPIC_NAME;
    topic.name = topic_name;
    topic.id = 0;
    
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    while (1) {
        publish_status();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
    return 0;
}