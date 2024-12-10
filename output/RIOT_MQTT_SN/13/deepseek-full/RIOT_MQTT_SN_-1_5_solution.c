
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <msg.h>
#include <unistd.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t remote;

int connect_to_broker(void) {
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid address/ Address not supported \n");
        return -1;
    }

    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Connection to MQTT-SN broker failed\n");
        return -1;
    }
    printf("Connected to MQTT-SN broker\n");
    return 0;
}

int register_topic(void) {
    topic.name = TOPIC_NAME;
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Topic registration failed\n");
        return -1;
    }
    printf("Topic registered with ID: %d\n", topic.id);
    return 0;
}

void publish_state(void) {
    int res = emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state\n");
    } else {
        printf("State published\n");
    }
}

int main(void) {
    if (connect_to_broker() != 0) {
        return -1;
    }

    if (register_topic() != 0) {
        emcute_discon();
        return -1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}
