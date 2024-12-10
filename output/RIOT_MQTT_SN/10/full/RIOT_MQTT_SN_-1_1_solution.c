#include "xtimer.h"
#include "net/emcute.h"
#include "string.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds
#define MQTT_TOPIC       "state"

static emcute_topic_t topic;

void publish_state(emcute_topic_t *topic) {
    const char* state = "work"; // For simplicity, we'll alternate in real applications based on a condition.

    while (1) {
        int res = emcute_pub(topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Publication attempt failed: %d\n", res);
        } else {
            printf("Published: %s\n", state);
        }
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
}

int main(void) {
    int res;
    emcute_sub_t subscriptions[5];
    emcute_con_t client;
    memset(&client, 0, sizeof(emcute_con_t));
    memset(subscriptions, 0, sizeof(subscriptions));

    // Set broker address
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    // Connect to broker
    while (1) {
        res = emcute_connect(&gw, false, "esp32-client");
        if (res != EMCUTE_OK) {
            printf("Connection attempt failed: %d\n", res);
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        break;
    }

    // Set topic
    topic.name = MQTT_TOPIC;
    res = emcute_reg(&topic); // Register topic
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Start publishing
    publish_state(&topic);

    return 0;
}