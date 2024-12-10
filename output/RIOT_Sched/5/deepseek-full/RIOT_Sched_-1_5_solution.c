
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <event/periodic_callback.h>
#include <thread.h>
#include <sched.h>

#define THREAD_STACKSIZE_DEFAULT 512

void thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        printf("Thread 1 priority: %d\n", thread_get_priority(thread_get_active()));
        sleep(1);
    }
}

void thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        printf("Thread 2 priority: %d\n", thread_get_priority(thread_get_active()));
        sleep(1);
    }
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1, 0, (thread_task_func_t)thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2, 0, (thread_task_func_t)thread_func2, NULL, "thread2");

    sleep(5);

    printf("Changing priority of thread 1 to higher\n");
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 3);

    sleep(5);

    printf("Changing priority of thread 2 to higher\n");
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 4);

    return 0;
}
