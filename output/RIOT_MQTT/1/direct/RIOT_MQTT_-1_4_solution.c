#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/af.h>
#include <net/emcute.h>
#include <net/conn/netdev.h>
#include <net/ipv6/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds
#define TOPIC "device/state"

static emcute_sock_t sock;

static int mqtt_init(void) {
    sock = emcute_sock_create(BROKER_IP, BROKER_PORT);
    if (sock < 0) {
        printf("[Error] MQTT connection failed\n");
        return -1;
    }
    printf("[Info] Connected to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    return 0;
}

static void publish_state(const char *state) {
    emcute_msg_t msg;
    msg.data = (void *)state;
    msg.len = strlen(state);
    emcute_publish(sock, TOPIC, &msg, EMCUTE_QOS_1);
    printf("[Info] Published state: %s\n", state);
}

int main(void) {
    if (mqtt_init() != 0) {
        return -1;
    }

    while (1) {
        publish_state("work");
        xtimer_usleep(PUB_INTERVAL);

        publish_state("not work");
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}