#include "thread_config.h"
#include "paho_mqtt.h"
#include "architecture.h"
#include "thread_arch.h"
#include "MQTTClient.h"

#include <stdio.h>
#include <mutex.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN PRIORITY_MAIN

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    printf("Starting threads...\n");

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                                         thread_func1, NULL, "thread 1");

    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0,
                                         thread_func2, NULL, "thread 2");

    xtimer_sleep(5);
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 1);

    printf("Changed priorities of threads\n");
    return 0;
}