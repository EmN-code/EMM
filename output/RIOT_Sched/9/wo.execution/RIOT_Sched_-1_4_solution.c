#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "architecture.h"

#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define PRIO_MAIN (THREAD_PRIORITY_MAIN - 1)
#define PRIO_SECOND (THREAD_PRIORITY_MAIN - 2)

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void thread_function(void *arg) {
    (void)arg;

    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
}

void thread_function2(void *arg) {
    (void)arg;

    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIO_MAIN, 0, thread_function, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 1\n");
        return 1;
    }

    pid2 = thread_create(stack2, sizeof(stack2), PRIO_SECOND, 0, thread_function2, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 2\n");
        return 1;
    }

    /* Let the main thread change the priority of thread 2 after some time */
    xtimer_sleep(5);
    printf("Changing priority of thread 2\n");
    sched_change_priority(thread_get(pid2), PRIO_MAIN + 1);

    return 0;
}