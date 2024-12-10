
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <unistd.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>

#define STACKSIZE 512
#define PRIORITY 5

kernel_pid_t thread1_pid, thread2_pid;

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE], stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, STACKSIZE, PRIORITY, 0, thread_func1, NULL, "thread1");
    thread2_pid = thread_create(stack2, STACKSIZE, PRIORITY, 0, thread_func2, NULL, "thread2");

    ztimer_sleep(ZTIMER_SEC, 5);

    uint8_t prio1 = thread_get_priority(thread1_pid);
    uint8_t prio2 = thread_get_priority(thread2_pid);

    printf("Initial priorities: Thread 1: %d, Thread 2: %d\n", prio1, prio2);

    thread_set_priority(thread1_pid, prio1 + 1);
    thread_set_priority(thread2_pid, prio2 - 1);

    prio1 = thread_get_priority(thread1_pid);
    prio2 = thread_get_priority(thread2_pid);

    printf("Changed priorities: Thread 1: %d, Thread 2: %d\n", prio1, prio2);

    return 0;
}
