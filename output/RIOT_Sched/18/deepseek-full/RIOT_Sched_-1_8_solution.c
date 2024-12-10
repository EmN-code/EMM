
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <timex.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <msg.h>
#include <event/thread.h>
#include <ztimer.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define STACKSIZE (1024)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];
static kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 running at priority: %d\n", thread_get_priority(pid1));
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 running at priority: %d\n", thread_get_priority(pid2));
    return NULL;
}

int main(void)
{
    puts("Starting two threads with different priorities...");

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    if (pid1 <= KERNEL_PID_UNDEF || pid2 <= KERNEL_PID_UNDEF) {
        puts("Failed to create threads");
        return 1;
    }

    ztimer_sleep(ZTIMER_MSEC, 1000);

    printf("Changing priority of Thread 1 to LOW...\n");
    thread_set_priority(pid1, PRIORITY_LOW);
    printf("New priority of Thread 1: %d\n", thread_get_priority(pid1));

    printf("Changing priority of Thread 2 to HIGH...\n");
    thread_set_priority(pid2, PRIORITY_HIGH);
    printf("New priority of Thread 2: %d\n", thread_get_priority(pid2));

    return 0;
}
