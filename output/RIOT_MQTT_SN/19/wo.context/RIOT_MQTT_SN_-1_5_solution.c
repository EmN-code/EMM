#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <stdio.h>
#include <xtimer.h>
#include <emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds
#define MQTT_TOPIC "device/state"

static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Published data: %.*s\n", (int)len, (char *)data);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr);

    char *topic_name = MQTT_TOPIC;
    topic.name = topic_name;
    topic.id = 0;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker\n");
        return -1;
    }

    printf("Connected to MQTT-SN broker\n");

    while (1) {
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish
");
        }
        else {
            printf("Published state: %s\n", state);
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}