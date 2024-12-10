
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <unistd.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 1024
#define PRIORITY THREAD_PRIORITY_MAIN - 1

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work";
    int res;

    while (1) {
        res = emcute_pub(&topic, state, strlen(state), 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        } else {
            printf("State published: %s\n", state);
        }

        xtimer_sleep(5);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    int res;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    res = inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Failed to parse server IP\n");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        emcute_discon();
        return 1;
    }

    thread_create(stack, STACK_SIZE, PRIORITY, 0, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
