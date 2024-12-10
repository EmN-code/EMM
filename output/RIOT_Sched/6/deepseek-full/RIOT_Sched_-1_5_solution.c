
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <MQTTClient.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdatomic.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <paho_mqtt.h>
#include <ztimer.h>

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_func2, NULL, "thread2");

    ztimer_sleep(ZTIMER_SEC, 5);

    sched_change_priority(pid1, THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN + 1);

    return 0;
}
