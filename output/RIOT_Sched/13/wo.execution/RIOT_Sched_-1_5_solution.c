#include "netinet/in.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "unistd.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "sched.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char thread_stack1[THREAD_STACKSIZE_MAIN];
static char thread_stack2[THREAD_STACKSIZE_MAIN];

void thread_func1(void *arg) {
    (void) arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
}

void thread_func2(void *arg) {
    (void) arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
}

int main(void) {
    kernel_pid_t pid1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN, CREATE_STACKTEST, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 1, CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change priority of thread 2
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 2);

    while (1) {
        printf("Main thread running\n");
        xtimer_sleep(2);
    }
    return 0;
}