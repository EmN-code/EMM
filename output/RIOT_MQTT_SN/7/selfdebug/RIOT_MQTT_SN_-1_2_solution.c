#include "xtimer.h"
#include "string.h"
#include "stdio.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT     1883
#define BROKER_IP       "47.102.103.1"
#define MQTT_TOPIC      "state"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t t;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received publication on topic %i of length %i\n", topic->id, (int)len);
}

static int init_mqtt(void) {
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("ERROR: Unable to parse broker IP");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: Unable to connect to broker");
        return 1;
    }

    t.name = MQTT_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("ERROR: Unable to register topic");
        return 1;
    }

    return 0;
}

static void publish_state(const char *state) {
    if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("ERROR: Unable to publish data");
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    if (init_mqtt() != 0) {
        return 1;
    }

    char *states[] = {"work", "not work"};
    size_t idx = 0;

    while (1) {
        publish_state(states[idx]);
        idx = (idx + 1) % 2;
        xtimer_sleep(PUBLISH_INTERVAL / 1000000);
    }

    return 0;
}