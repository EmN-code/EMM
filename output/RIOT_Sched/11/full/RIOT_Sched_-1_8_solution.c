#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"

// Forward declarations of functions.
void *thread_handler_one(void *arg);
void *thread_handler_two(void *arg);

int main(void) {
    // Define stack sizes and priorities for threads.
    const int stack_size = 1024;
    static char stack_one[stack_size];
    static char stack_two[stack_size];
    int priority_one = THREAD_PRIORITY_MAIN - 1;
    int priority_two = THREAD_PRIORITY_MAIN - 2;

    // Create threads.
    kernel_pid_t thread_one = thread_create(stack_one, sizeof(stack_one),
                                             priority_one, 0,
                                             thread_handler_one, NULL,
                                             "Thread-One");
    kernel_pid_t thread_two = thread_create(stack_two, sizeof(stack_two),
                                             priority_two, 0,
                                             thread_handler_two, NULL,
                                             "Thread-Two");

    // Simulate runtime priority change.
    sched_change_priority(thread_get(thread_one), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_MAIN - 4);

    // Main loop.
    while (1) {
        // Main thread loop...
    }

    return 0;
}

void *thread_handler_one(void *arg) {
    while (1) {
        // Handler code for thread one...
    }
    return NULL;
}

void *thread_handler_two(void *arg) {
    while (1) {
        // Handler code for thread two...
    }
    return NULL;
}
