#include <unistd.h>
#include <thread.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <ztimer.h>

// Define stack sizes for threads
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

// Thread stack and priority
char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

void *thread1_func(void *arg) {
    while (1) {
        // Simulate work in thread1
        printf("Thread 1 is running\n");
        sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    while (1) {
        // Simulate work in thread2
        printf("Thread 2 is running\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create threads
    pid1 = thread_create(stack1, THREAD_STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST,
                         thread1_func, NULL, "thread1");

    pid2 = thread_create(stack2, THREAD_STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST,
                         thread2_func, NULL, "thread2");

    // Change the priority of thread 1 after start
    if (pid1 >= 0) {
        thread_set_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    }

    // Change the priority of thread 2 after start
    if (pid2 >= 0) {
        thread_set_priority(pid2, THREAD_PRIORITY_MAIN - 3);
    }

    while (1) {
        // Main thread activity
        printf("Main thread is running\n");
        sleep(2);
    }

    return 0;
}