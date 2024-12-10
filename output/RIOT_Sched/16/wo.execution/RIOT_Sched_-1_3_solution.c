#include <unistd.h>
#include <time_units.h>
#include <thread_arch.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sched.h>
#include <MQTTClient.h>
#include <dht_params.h>
#include <architecture.h>
#include <event/thread.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_IDLE  (THREAD_PRIORITY_IDLE + 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 1 is running");
        usleep(1000000);  // Sleep for 1 second
    }
    return NULL;
}

static void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 2 is running");
        usleep(1500000);  // Sleep for 1.5 seconds
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                                              thread_func1, NULL, "thread1");
                                               
    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1, 0,
                                              thread_func2, NULL, "thread2");

    // Change priority of threads
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN + 3);

    return 0;
}
