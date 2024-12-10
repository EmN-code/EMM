#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/emcute.h"
#include "net/af.h"
#include "arpa/inet.h"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

#define BROKER_IP "47.102.103.1"
#define MQTT_TOPIC "device/state"

void publish_state(void) {
    const char *states[] = { "work", "not work" };
    static bool work_state = true;
    char *state = work_state ? states[0] : states[1];
    work_state = !work_state;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
        return;
    }

    printf("Published %s to topic %s\n", state, MQTT_TOPIC);
}

int main(void) {
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse broker IP");
        return -1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return -1;
    }

    strncpy(topic.name, MQTT_TOPIC, sizeof(topic.name));

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}