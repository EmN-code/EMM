
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "msg.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8

static msg_t _msg_queue[MSG_QUEUE_SIZE];

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return;
    }

    const char *state = states[state_index];
    res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    } else {
        printf("Published: %s\n", state);
    }

    state_index = (state_index + 1) % 2;
}

int main(void) {
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    int res = inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Error parsing IP address\n");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
