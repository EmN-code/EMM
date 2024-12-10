#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/mqttsn.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <net/ipv6/addr.h> // For inet_pton function

#define MQTT_TOPIC "state"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL 5
#define BUFFER_SIZE 64

static emcute_topic_t pub_topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published %u bytes on topic "%s"
", len, topic->name);
}

int main(void) {
    sockaddr_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr);

    emcute_sub_t subscriptions[1];
    memset(subscriptions, 0, sizeof(subscriptions));

    pub_topic.name = MQTT_TOPIC;

    printf("Connecting to MQTT-SN broker %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        snprintf(buffer, sizeof(buffer), "work");
        emcute_pub(&pub_topic, buffer, strlen(buffer), EMCUTE_QOS_0);
        on_pub(&pub_topic, buffer, strlen(buffer));
        xtimer_sleep(PUB_INTERVAL);
    }

    emcute_discon();
    return 0;
}