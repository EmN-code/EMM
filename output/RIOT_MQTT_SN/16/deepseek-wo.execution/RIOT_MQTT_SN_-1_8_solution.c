
#include <net/emcute.h>
#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void publish_state(emcute_topic_t *topic) {
    const char *state = "work";
    if (rand() % 2) {
        state = "not work";
    }
    emcute_pub(topic, state, strlen(state), 0);
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    sock_udp_ep_t gw = { .family = AF_INET, .port = HTONS(BROKER_PORT) };
    inet_aton(BROKER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state(&topic);
        xtimer_sleep(5);
    }

    return 0;
}
