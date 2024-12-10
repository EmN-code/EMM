
#include "periph/i2c.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "stddef.h"
#include "netinet/in.h"
#include "xtimer.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "periph/rtc.h"
#include "timex.h"
#include "stdio.h"
#include "mbox.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "time_units.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "periph/gpio.h"
#include "sched.h"
#include "event/periodic_callback.h"
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT (1024)

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func2, NULL, "thread2");

    xtimer_sleep(2); // Wait for threads to start

    // Change priority of thread 1
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 4);

    printf("Main thread exiting\n");
    return 0;
}
