
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <ztimer.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static kernel_pid_t pid1, pid2;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(pid1));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(pid2));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void change_priority(kernel_pid_t pid, uint8_t priority)
{
    if (thread_set_priority(pid, priority) == 0) {
        printf("Priority of thread %d changed to %d\n", pid, priority);
    } else {
        printf("Failed to change priority of thread %d\n", pid);
    }
}

int main(void)
{
    puts("Starting two threads with priority change at runtime");

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    if (pid1 <= KERNEL_PID_UNDEF || pid2 <= KERNEL_PID_UNDEF) {
        puts("Failed to create threads");
        return 1;
    }

    ztimer_sleep(ZTIMER_MSEC, 5000);
    change_priority(pid1, PRIORITY_LOW);
    change_priority(pid2, PRIORITY_HIGH);

    ztimer_sleep(ZTIMER_MSEC, 5000);
    change_priority(pid1, PRIORITY_HIGH);
    change_priority(pid2, PRIORITY_LOW);

    return 0;
}
