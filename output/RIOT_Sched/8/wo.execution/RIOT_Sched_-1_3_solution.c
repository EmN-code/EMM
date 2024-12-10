// Description:
// The following code sets up two threads on an ESP32 using RIOT OS. One thread changes the priority of the other at runtime.
// The code ensures that the THREAD_STACKSIZE_DEFAULT is only defined once to avoid redefinition errors.

// Imports in descending alphabetical order.
#include <unistd.h>
#include <time_units.h>
#include <thread_config.h>
#include <thread_arch.h>
#include <thread.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sched.h>
#include <architecture.h>
#include <MQTTClient.h>
#include <event/thread.h>

#define MAIN_THREAD_STACKSIZE 1024
#define THREAD1_STACKSIZE 1024
#define THREAD2_STACKSIZE 1024

static char thread1_stack[THREAD1_STACKSIZE];
static char thread2_stack[THREAD2_STACKSIZE];

void *thread1_handler(void *arg) {
    (void)arg;
    while (1) {
        // Thread 1 performs its tasks
        printf("Thread 1 is running\n");
        usleep(1000000);
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    thread_get_unregister(thread_getpid());
    // Change the priority of Thread 1
    printf("Thread 2 changing priority of Thread 1\n");
    sched_change_priority(thread_get(thread_getpid_of(thread1_stack)), 2);
    while (1) {
        // Thread 2 performs its tasks
        printf("Thread 2 is running\n");
        usleep(1000000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), 2, THREAD_CREATE_JOINABLE, thread1_handler, NULL, "thread1");
    if (thread1_pid < 0) {
        printf("Error: unable to create thread 1\n");
        return 1;
    }

    kernel_pid_t thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), 3, THREAD_CREATE_JOINABLE, thread2_handler, NULL, "thread2");
    if (thread2_pid < 0) {
        printf("Error: unable to create thread 2\n");
        return 1;
    }

    while (1) {
        printf("Main thread is running\n");
        usleep(1000000);
    }

    return 0;
}