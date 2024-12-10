#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "architecture.h"

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

static char stack1[STACK_SIZE];
static char stack2[STACK_SIZE];
kernel_pid_t pid1, pid2;

void* thread_handler(void* arg) {
    while (1) {
        xtimer_sleep(1);
        printf("Thread %d running\n", (int)arg);
    }
    return NULL;
}

int main(void) {
    printf("Starting the RIOT application\n");

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY, 0, thread_handler, (void *)1, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY, 0, thread_handler, (void *)2, "thread2");

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    while (1) {
        xtimer_sleep(5);
        printf("Main thread running\n");
    }

    return 0;
}