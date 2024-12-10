
#include "net/emcute.h"
#include "xtimer.h"
#include "net/ipv6/addr.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;

int connect_to_broker(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: invalid IP address");
        return -1;
    }

    result = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (result != EMCUTE_OK) {
        printf("Error: could not connect to MQTT-SN broker [%d]\n", result);
        return -1;
    }

    return 0;
}

int register_topic(void) {
    topic.name = TOPIC_NAME;
    int result = emcute_reg(&topic);
    if (result != EMCUTE_OK) {
        printf("Error: could not register topic [%d]\n", result);
        return -1;
    }

    return 0;
}

void publish_state(bool state) {
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    int result = emcute_pub(&topic, msg, strlen(msg), 0);
    if (result != EMCUTE_OK) {
        printf("Error: could not publish message [%d]\n", result);
    }
}

int main(void) {
    if (connect_to_broker() != 0) {
        return -1;
    }

    if (register_topic() != 0) {
        return -1;
    }

    bool state = true;
    while (1) {
        publish_state(state);
        state = !state;
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
