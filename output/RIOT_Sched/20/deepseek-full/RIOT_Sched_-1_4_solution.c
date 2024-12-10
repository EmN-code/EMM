
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "thread.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started\n");
    xtimer_sleep(5);
    printf("Thread 1 finished\n");
    return NULL;
}

static void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started\n");
    xtimer_sleep(5);
    printf("Thread 2 finished\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    printf("Starting main thread\n");

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1_func, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2_func, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    xtimer_sleep(2);

    printf("Changing priorities of threads\n");
    thread_set_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    thread_set_priority(pid2, THREAD_PRIORITY_MAIN - 2);

    printf("Main thread finished\n");
    return 0;
}
