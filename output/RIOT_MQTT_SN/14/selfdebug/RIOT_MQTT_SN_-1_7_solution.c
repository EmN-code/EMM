#include <stdio.h>
#include "net/ipv6/addr.h"  // Ensure this is included to declare inet_pton
#include "net/emcute.h"

#define EMCUTE_PORT         1885
#define MQTT_TOPIC          "work/status"
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_INTERVAL           (5U)

static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;

    printf("Published data to topic: %s\n", topic->name);
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    int result = inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert broker address\n");
        return 1;
    }

    emcute_sub_t subscription;
    subscription.topic.name = MQTT_TOPIC;
    subscription.cb = on_pub;
    unsigned flags = EMCUTE_QOS_0;

    // Connect to broker
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    // Register topic
    topic.name = MQTT_TOPIC;
    emcute_reg(&topic);

    // Setup periodic publishing loop
    while (1) {
        emcute_pub(&topic, "work", 4, flags);
        xtimer_sleep(MQTT_INTERVAL);
    }

    return 0;
}