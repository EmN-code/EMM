#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "sched.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)

char stack_one[THREAD_STACKSIZE];
char stack_two[THREAD_STACKSIZE];

void *thread_one(void *arg) {
    int count = 0;
    while (1) {
        printf("Thread One: %d\n", count++);
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

void *thread_two(void *arg) {
    int count = 0;
    while (1) {
        printf("Thread Two: %d\n", count++);
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

void change_thread_priority(kernel_pid_t pid, uint8_t new_priority) {
    irq_disable();
    sched_change_priority(thread_get(pid), new_priority);
    irq_enable();
}

int main(void) {
    printf("Starting threads with different priorities\n");
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY, 0,
                                          thread_one, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY + 1, 0,
                                          thread_two, NULL, "thread_two");

    ztimer_sleep(ZTIMER_USEC, 5000000);

    printf("Switching thread priorities\n");
    change_thread_priority(pid_one, THREAD_PRIORITY + 1);
    change_thread_priority(pid_two, THREAD_PRIORITY);

    while (1) {
        ztimer_sleep(ZTIMER_USEC, 5000000);
    }

    return 0;
}