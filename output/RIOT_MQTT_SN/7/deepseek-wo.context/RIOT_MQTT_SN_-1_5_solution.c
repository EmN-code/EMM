
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define TOPIC_NAME "work_or_not_work"
#define BUF_SIZE 20
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

static char buf[BUF_SIZE];
static emcute_topic_t topic;

void publish_state(void) {
    static int state = 0;
    const char *state_str = state ? STATE_WORK : STATE_NOT_WORK;
    state = !state;

    int res = emcute_pub(&topic, state_str, strlen(state_str), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    } else {
        printf("Published state: %s\n", state_str);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = 1883, .family = AF_INET };
    inet_addr_from_str(&gw.addr.ipv4, "47.102.103.1");

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name) - 1);
    topic.name[sizeof(topic.name) - 1] = '\0';
    topic.id = 0;

    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
