
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
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <ztimer.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 3

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

static void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;
    uint8_t priority1, priority2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    if (pid1 <= KERNEL_PID_UNDEF || pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 5);

        priority1 = thread_get_priority(pid1);
        priority2 = thread_get_priority(pid2);

        printf("Current priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

        if (priority1 != PRIORITY_LOW) {
            thread_set_priority(pid1, PRIORITY_LOW);
            thread_set_priority(pid2, PRIORITY_HIGH);
            printf("Priorities swapped\n");
        }
    }

    return 0;
}
