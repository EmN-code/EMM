#include "xtimer.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "timex.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND  (THREAD_PRIORITY_MAIN - 2)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running...\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running...\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(thread_stack_one, sizeof(thread_stack_one),
                                         THREAD_PRIORITY_MAIN, 0,
                                         thread_one, NULL, "thread_one");

    kernel_pid_t pid_two = thread_create(thread_stack_two, sizeof(thread_stack_two),
                                         THREAD_PRIORITY_SECOND, 0,
                                         thread_two, NULL, "thread_two");

    (void)pid_one;
    (void)pid_two;

    xtimer_sleep(3);
    printf("Changing priority of Thread 1\n");

    thread_t *t1 = thread_get(pid_one);
    if (t1) {
        sched_change_priority(t1, THREAD_PRIORITY_SECOND);
    }

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}