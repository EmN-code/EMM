#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define APP_MAIN_QUEUE_SIZE (4)
#define EMCUTE_ID "esp32"
#define TOPIC "state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[APP_MAIN_QUEUE_SIZE];
static emcute_topic_t t;

static void *emcute_thread(void *arg) {
    (void)arg;
    char *work_states[2] = {"not work", "work"};
    int state_index = 0;
    
    while (1) {
        const char *state = work_states[state_index];
        printf("Publishing state: %s\n", state);
        emcute_pub(&t, state, strlen(state), EMCUTE_QOS_1);

        state_index = (state_index + 1) % 2;
        xtimer_sleep(5);
    }
    
    return NULL;
}

int main(void) {
    msg_init_queue(queue, APP_MAIN_QUEUE_SIZE);

    /* Setup emcute thread stack and priority */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(BROKER_PORT)
    };
    inet_pton(AF_INET, BROKER_IP, &broker_addr.sin_addr);

    if (emcute_sock(&broker_addr) != 0) {
        puts("Error: unable to connect to broker\n");
        return 1;
    }

    /* Publish to the topic "state" initially */
    t.name = TOPIC;
    emcute_sub(&t);

    while (1) {
        thread_yield();
    }

    return 0;
}