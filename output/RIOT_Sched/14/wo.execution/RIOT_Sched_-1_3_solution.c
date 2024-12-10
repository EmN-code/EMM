#include "thread_config.h"
#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "kernel_defines.h"
#include <stdatomic.h>
#include <sched.h>
#include <MQTTClient.h>
#include <event/thread.h>

#define STACK_SIZE_THREAD    (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_THREAD_A    (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD_B    (THREAD_PRIORITY_MAIN - 2)

char stack_thread_a[STACK_SIZE_THREAD];
char stack_thread_b[STACK_SIZE_THREAD];

void *thread_a(void *arg) {
    while (1) {
        printf("Thread A running with priority: %d\n", PRIORITY_THREAD_A);
        sleep(1);
    }
    return NULL;
}

void *thread_b(void *arg) {
    while (1) {
        printf("Thread B running with priority: %d\n", PRIORITY_THREAD_B);
        sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_thread_a = thread_create(
        stack_thread_a,
        sizeof(stack_thread_a),
        PRIORITY_THREAD_A,
        THREAD_CREATE_STACKTEST,
        thread_a,
        NULL,
        "thread_a"
    );

    kernel_pid_t pid_thread_b = thread_create(
        stack_thread_b,
        sizeof(stack_thread_b),
        PRIORITY_THREAD_B,
        THREAD_CREATE_STACKTEST,
        thread_b,
        NULL,
        "thread_b"
    );

    // Change priorities at runtime
    sched_change_priority(thread_get(pid_thread_a), PRIORITY_THREAD_B);
    sched_change_priority(thread_get(pid_thread_b), PRIORITY_THREAD_A);

    while (1) {
        printf("Main thread: priorities swapped\n");
        sleep(3);
    }
    return 0;
}