
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <stdatomic.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <event/thread.h>
#include <esp_system.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <esp_wifi.h>
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

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

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

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread_func2, NULL, "thread2");

    printf("Threads created with PIDs: %d and %d\n", pid1, pid2);

    // Change priority of thread 1
    thread_set_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread 1 to %d\n", THREAD_PRIORITY_MAIN - 2);

    // Change priority of thread 2
    thread_set_priority(pid2, THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 2 to %d\n", THREAD_PRIORITY_MAIN - 3);

    return 0;
}
