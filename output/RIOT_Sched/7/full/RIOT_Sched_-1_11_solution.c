#include "net/sock/util.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "architecture.h"

// Define thread stack sizes and priorities
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_SMALL   (THREAD_STACKSIZE_DEFAULT / 2)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND   (THREAD_PRIORITY_MAIN - 2)

// Thread function prototypes
void *thread_func_one(void *arg);
void *thread_func_two(void *arg);

// Thread stacks
char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

// Thread control blocks
thread_t thread_one;
thread_t thread_two;

// Thread function implementations
void *thread_func_one(void *arg) {
    (void)arg;
    while (1) {
        // Simulated work for thread one
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    (void)arg;
    while (1) {
        // Simulated work for thread two
    }
    return NULL;
}

int main(void) {
    // Create and start thread one
    thread_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, 0, thread_func_one, NULL, "thread_one");
    if (thread_one <= KERNEL_PID_UNDEF) {
        // Handle thread creation failure
    }

    // Create and start thread two
    thread_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_SECOND, 0, thread_func_two, NULL, "thread_two");
    if (thread_two <= KERNEL_PID_UNDEF) {
        // Handle thread creation failure
    }

    // Simulate runtime priority change
    // Increase the priority of thread two
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_MAIN + 1);

    // Main loop
    while (1) {
        // Simulate main work
    }

    return 0;
}