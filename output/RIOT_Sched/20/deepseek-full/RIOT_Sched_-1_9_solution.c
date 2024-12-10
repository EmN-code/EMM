
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <esp_system.h>
#include <stddef.h>
#include <stdbool.h>
#include <mutex.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <mbox.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <thread.h>
#include <net/emcute.h>
#include <unistd.h>
#include <stdatomic.h>
#include <MQTTClient.h>

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        printf("Thread 1 working\n");
        sleep(1);
    }
    return NULL;
}

static void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        printf("Thread 2 working\n");
        sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    puts("Starting two threads...");

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2_func, NULL, "thread2");

    if (pid1 <= KERNEL_PID_UNDEF || pid2 <= KERNEL_PID_UNDEF) {
        puts("Error creating threads");
        return 1;
    }

    puts("Threads created successfully");

    // Change priority of thread1 to higher
    thread_set_priority(pid1, PRIORITY - 1);
    puts("Thread 1 priority changed to higher");

    // Change priority of thread2 to lower
    thread_set_priority(pid2, PRIORITY + 1);
    puts("Thread 2 priority changed to lower");

    while (1) {
        sleep(10);
    }

    return 0;
}
