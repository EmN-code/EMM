#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define SENSOR_DELAY        (5U)
#define MQTT_BROKER         "47.102.103.1"
#define MQTT_PORT           (1883U)

static emcute_topic_t state_topic;
static sock_udp_ep_t mqtt_server = {
    .family = AF_INET,
    .port = MQTT_PORT
};

static char* state = "work";

void publish_state(void) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&state_topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("Error: unable to publish data
");
    }
}

int main(void) {
    char addr[sizeof(struct in_addr)];
    if (inet_pton(AF_INET, MQTT_BROKER, addr) != 1) {
        printf("Error translating broker address
");
        return 1;
    }
    memcpy(&mqtt_server.addr, addr, sizeof(mqtt_server.addr));

    if (emcute_con(&mqtt_server, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error connecting to broker
");
        return 1;
    }

    state_topic.name = "state/working";
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Error registering topic
");
        return 1;
    }
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, SENSOR_DELAY * MS_PER_SEC);
    }
    return 0;
}