#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emcute.h>
#include <xtimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define NODE_NAME "esp32_node"
#define TOPIC_NAME "device/state"
#define PUB_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char mqtt_topic[64];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;  // suppress unused parameter warning
    printf("Published data: %.*s\n", (int)len, (char *)data);
}

static int init_mqtt(void) {
    emcute_conf_t conf = {
        .id = NODE_NAME,
        .port = MQTT_BROKER_PORT,
        .address = MQTT_BROKER_IP
    };

    if (emcute_sock(&conf) != EMCUTE_OK) {
        puts("Failed to initialize emcute");
        return 1;
    }

    emcute_cb_t cb = on_pub;
    size_t msg_len = snprintf(mqtt_topic, sizeof(mqtt_topic), "%s", TOPIC_NAME);

    if (msg_len < 0 || msg_len >= sizeof(mqtt_topic)) {
        puts("Failed to format topic");
        return 1;
    }

    topic.name = mqtt_topic;
    topic.id = 0;

    if (emcute_sub(&topic, 1, cb) != EMCUTE_OK) {
        puts("Failed to subscribe to topic");
        return 1;
    }

    return 0;
}

static void publish_state(const char *state) {
    int result = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (result != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", result);
    }
}

int main(void) {
    puts("MQTT-SN client example");

    if (init_mqtt() != 0) {
        return 1;
    }

    while (1) {
        publish_state("work");
        xtimer_sleep(PUB_INTERVAL);
        publish_state("not work");
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}