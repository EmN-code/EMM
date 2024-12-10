#include "unistd.h"
#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "MQTTClient.h"
#include "event/thread.h"

#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY 5

char thread_stack_one[THREAD_STACKSIZE];
char thread_stack_two[THREAD_STACKSIZE];
kernel_pid_t pid_one;
kernel_pid_t pid_two;

void *thread_function_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        usleep(1000000);
    }
    return NULL;
}

void *thread_function_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        usleep(1000000);
    }
    return NULL;
}

int main(void) {
    pid_one = thread_create(thread_stack_one, THREAD_STACKSIZE, THREAD_PRIORITY, 0, thread_function_one, NULL, "thread_one");
    pid_two = thread_create(thread_stack_two, THREAD_STACKSIZE, THREAD_PRIORITY, 0, thread_function_two, NULL, "thread_two");

    usleep(5000000); // Let threads run a bit

    // Change thread priorities
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 1); // Increase priority
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN + 1); // Decrease priority

    while (1) {
        usleep(1000000);
    }
    return 0;
}