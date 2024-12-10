#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include <stdio.h>

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)
#define STACKSIZE            (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE       (8)

char stack_low[STACKSIZE];
char stack_high[STACKSIZE];

static msg_t msg_queue_low[MSG_QUEUE_SIZE];
static msg_t msg_queue_high[MSG_QUEUE_SIZE];

static void *high_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        if (msg_receive(&msg)) {
            printf("High Priority Thread received msg content: %d\n", msg.content.value);
        }
    }

    return NULL;
}

static void *low_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Low Priority Thread received msg content: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    /* Create threads */
    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high_priority_thread");
    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_priority_thread");

    /* Send messages to the high priority thread */
    for (int i = 0; i < 5; i++) {
        msg_t msg;
        msg.content.value = i;
        msg_send(&msg, high_pid);
        xtimer_sleep(1);
    }

    /* Send messages to the low priority thread */
    for (int i = 0; i < 5; i++) {
        msg_t msg;
        msg.content.value = i + 100;
        msg_send(&msg, low_pid);
        xtimer_sleep(1);
    }

    return 0;
}