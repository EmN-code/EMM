
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>

#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define STACKSIZE 512
#define NUM_MESSAGES 16

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];
static msg_t msg_queue[NUM_MESSAGES];

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                      THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    thread_t *thread1 = thread_get_from_pid(pid1);
    thread_t *thread2 = thread_get_from_pid(pid2);

    thread_set_priority(thread1, THREAD_PRIORITY_MAIN - 2);
    thread_set_priority(thread2, THREAD_PRIORITY_MAIN - 1);

    printf("Priorities changed\n");

    return 0;
}
