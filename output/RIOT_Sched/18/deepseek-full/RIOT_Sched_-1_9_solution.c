
#include "esp_system.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

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
    uint8_t priority1, priority2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread_func2, NULL, "thread2");

    if (pid1 <= KERNEL_PID_UNDEF || pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    ztimer_sleep(ZTIMER_MSEC, 5000);

    priority1 = thread_get_priority(pid1);
    priority2 = thread_get_priority(pid2);

    printf("Initial priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

    thread_set_priority(pid1, PRIORITY - 1);
    thread_set_priority(pid2, PRIORITY + 1);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    priority1 = thread_get_priority(pid1);
    priority2 = thread_get_priority(pid2);

    printf("Updated priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

    return 0;
}
