
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <ztimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static sock_udp_ep_t remote;
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work";
    int res;

    res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    } else {
        printf("State published: %s\n", state);
    }
}

void connect_to_broker(void) {
    int res;

    remote.family = AF_INET;
    remote.port = HTONS(EMQX_SERVER_PORT);
    res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Error parsing IP address\n");
        return;
    }

    res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to broker: %d\n", res);
        return;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return;
    }

    printf("Connected to broker and topic registered\n");
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    connect_to_broker();

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
