
#include <arpa/inet.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <event/thread.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <thread.h>
#include <mbox.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <kernel_defines.h>

#define STACKSIZE (1024)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 running at priority: %d\n", thread_get_priority(thread_get_active()));
    while (1) {
        // Thread 1 logic
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 running at priority: %d\n", thread_get_priority(thread_get_active()));
    while (1) {
        // Thread 2 logic
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    // Create thread 1
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    if (pid1 <= 0) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    // Create thread 2
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");
    if (pid2 <= 0) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    // Change priority of thread 1 to low
    sched_change_priority(thread_get(pid1), PRIORITY_LOW);
    printf("Changed priority of thread 1 to low\n");

    // Change priority of thread 2 to high
    sched_change_priority(thread_get(pid2), PRIORITY_HIGH);
    printf("Changed priority of thread 2 to high\n");

    return 0;
}
