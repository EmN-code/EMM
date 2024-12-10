
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <ztimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void publish_state(void) {
    const char *state = "work or not work";
    emcute_pub(&topic, state, strlen(state), 0);
}

static void connect_to_broker(void) {
    if (inet_aton(BROKER_IP, &gw.addr.ipv4) == 0) {
        puts("Error: invalid broker IP address");
        return;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not connect to MQTT-SN broker [%d]\n", res);
        return;
    }

    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: could not register topic [%d]\n", res);
        return;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    puts("Starting MQTT-SN client");
    connect_to_broker();
    return 0;
}
