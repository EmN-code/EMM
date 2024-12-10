
#include "periph/i2c.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "stddef.h"
#include "xtimer.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "periph/rtc.h"
#include "timex.h"
#include "stdio.h"
#include "mbox.h"
#include "ztimer.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "periph/gpio.h"
#include "sched.h"
#include "event/periodic_callback.h"
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 1 to higher\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 1);

    xtimer_sleep(5);

    printf("Changing priority of Thread 2 to higher\n");
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN + 1);

    return 0;
}
