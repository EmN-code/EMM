
#include <stdio.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <msg.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return;
    }

    const char *state = states[state_index];
    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: could not publish message");
    } else {
        puts("State published");
    }

    state_index = (state_index + 1) % 2;
}

int main(void) {
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
