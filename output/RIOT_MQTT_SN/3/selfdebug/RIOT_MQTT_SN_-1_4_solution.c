#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000000U)  // 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Message published\n");
}

static int pub(char *topic_name, char *msg) {
    topic.name = topic_name;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("error: unable to publish");
        return 1;
    }
    
    return 0;
}

int main(void) {
    sock_udp_ep_t gw = { .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    char *state[] = {"work", "not work"};
    int current_state = 0;

    while (1) {
        pub("state", state[current_state]);
        current_state = 1 - current_state;  // Toggle state
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}