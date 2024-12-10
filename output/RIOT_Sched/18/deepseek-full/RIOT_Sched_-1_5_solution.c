
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <ztimer.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 3

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

static void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;
    uint8_t priority1, priority2;

    // Create the first thread with high priority
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    // Create the second thread with low priority
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    // Get the initial priorities
    priority1 = thread_get_priority(pid1);
    priority2 = thread_get_priority(pid2);
    printf("Initial priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

    // Change the priority of thread 1 to low after 5 seconds
    ztimer_sleep(ZTIMER_MSEC, 5000);
    thread_set_priority(pid1, PRIORITY_LOW);
    printf("Changed priority of Thread 1 to low\n");

    // Change the priority of thread 2 to high after another 5 seconds
    ztimer_sleep(ZTIMER_MSEC, 5000);
    thread_set_priority(pid2, PRIORITY_HIGH);
    printf("Changed priority of Thread 2 to high\n");

    // Get the updated priorities
    priority1 = thread_get_priority(pid1);
    priority2 = thread_get_priority(pid2);
    printf("Updated priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

    return 0;
}
