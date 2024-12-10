#include <stdlib.h> // Including essential library header for inet_pton
#include <net/ipv6/addr.h> // Required for ipv6 related address functions
#include <net/emcute.h> // Include emcute library
#include <stdio.h> // Standard I/O library
#include <string.h> // String manipulation library
#include "xtimer.h" // Timer utility from RIOT OS

#define EMCUTE_ID "device"
#define EMCUTE_PORT 1883
#define EMCUTE_TOPIC_ID 1234

static char status_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t status_queue[8];

static void *status_thread(void *arg) {
    static const char *state_msg[] = {"work", "not work"};
    emcute_topic_t state_topic;

    state_topic.name = "state";
    state_topic.id = EMCUTE_TOPIC_ID;

    while (1) {
        // Publish random state
        const char *msg = state_msg[rand() % 2];
        printf("Publishing: %s\n", msg);
        int res = emcute_pub(&state_topic, msg, strlen(msg), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error while publishing: %d\n", res);
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);

    msg_init_queue(status_queue, ARRAY_SIZE(status_queue));
    thread_create(status_stack, sizeof(status_stack), EMCUTE_PRIO, 0,
                    status_thread, NULL, "status");

    return 0;
}