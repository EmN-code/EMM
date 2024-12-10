
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MSG "work"

static emcute_topic_t topic;

void publish_state(void) {
    const char *msg = (xtimer_now().ticks32 % 2 == 0) ? "work" : "not work";
    emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
