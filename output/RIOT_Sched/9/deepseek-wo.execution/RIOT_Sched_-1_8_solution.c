
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
#include <esp_system.h>
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

#define THREAD_STACKSIZE_DEFAULT (1024)

static void *thread_func_1(void *arg)
{
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

static void *thread_func_2(void *arg)
{
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void)
{
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    printf("Threads created with PIDs: %d and %d\n", pid_1, pid_2);

    // Change priority of thread 1
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 1);
    printf("Changed priority of thread 1\n");

    // Change priority of thread 2
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN + 2);
    printf("Changed priority of thread 2\n");

    return 0;
}
