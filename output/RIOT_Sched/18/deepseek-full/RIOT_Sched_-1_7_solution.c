
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <timex.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <ztimer.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dht_params.h>

#define STACKSIZE (512)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;
    uint8_t priority1, priority2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread_func2, NULL, "thread2");

    if (pid1 <= KERNEL_PID_UNDEF || pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    priority1 = thread_get_priority(pid1);
    priority2 = thread_get_priority(pid2);

    printf("Initial priorities: Thread 1: %u, Thread 2: %u\n", priority1, priority2);

    thread_set_priority(pid1, PRIORITY + 1);
    thread_set_priority(pid2, PRIORITY + 2);

    priority1 = thread_get_priority(pid1);
    priority2 = thread_get_priority(pid2);

    printf("Updated priorities: Thread 1: %u, Thread 2: %u\n", priority1, priority2);

    return 0;
}
