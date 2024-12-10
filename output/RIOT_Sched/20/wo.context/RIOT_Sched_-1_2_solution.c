#include <thread_config.h>
#include <thread_arch.h>
#include <periph_conf.h>
#include <cpu_conf.h>
#include <stdio.h>
#include "thread.h"

#define THREAD_PRIORITY_MAIN    (7U)
#define THREAD_STACKSIZE        (THREAD_STACKSIZE_MAIN)

char stack_one[THREAD_STACKSIZE];
char stack_two[THREAD_STACKSIZE];

static kernel_pid_t pid_one;
static kernel_pid_t pid_two;

void *thread_function_one(void *arg) {
    (void)arg;
    printf("Thread one running with priority: %u\n", THREAD_PRIORITY_MAIN);
    while (1) {
        thread_yield();
    }
    return NULL;
}

void *thread_function_two(void *arg) {
    (void)arg;
    printf("Thread two running with priority: %u\n", THREAD_PRIORITY_MAIN + 1);
    while (1) {
        thread_yield();
    }
    return NULL;
}

int main(void) {
    puts("Starting thread creation\n");
    
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                            thread_function_one, NULL, "thread_one");
    if (pid_one > 0) {
        printf("Thread one created with PID: %d\n", pid_one);
    } else {
        printf("Error creating thread one\n");
    }

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
                            thread_function_two, NULL, "thread_two");
    if (pid_two > 0) {
        printf("Thread two created with PID: %d\n", pid_two);
    } else {
        printf("Error creating thread two\n");
    }

    // Changing thread priority
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN + 3);

    return 0;
}
