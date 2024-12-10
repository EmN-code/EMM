#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <arpa/inet.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/status"
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];
static msg_t queue[8];

static emcute_topic_t topic;

void *publish_state(void *arg) {
    (void)arg;
    const char *states[2] = {"work", "not work"};
    int state_idx = 0;

    while (1) {
        state_idx = (state_idx + 1) % 2;
        int res = emcute_pub(&topic, states[state_idx], strlen(states[state_idx]), EMCUTE_QOS_0);

        if (res != EMCUTE_OK) {
            printf("Error: Unable to publish data (%d)\n", res);
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    sockaddr_in6 remote_addr;
    inet_pton(AF_INET6, BROKER_ADDRESS, &remote_addr.sin6_addr);
    remote_addr.sin6_family = AF_INET6;
    remote_addr.sin6_port = htons(BROKER_PORT);

    emcute_con((sock_udp_ep_t *)&remote_addr, true, NULL, NULL, 0, 0);

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name) - 1);
    topic.name[sizeof(topic.name) - 1] = '\0';

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to register topic (%d)\n", res);
        return 1;
    }

    pthread_t thread;
    pthread_create(&thread, NULL, publish_state, NULL);

    emcute_run(BROKER_PORT, "esp32Client");

    return 0;
}