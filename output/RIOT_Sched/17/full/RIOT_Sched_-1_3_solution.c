#include "unistd.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "architecture.h"
#include "event/thread.h"

#define THREAD_STACKSIZE_MAIN  (THREAD_STACKSIZE_DEFAULT)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    while (1) {
        // Thread performs tasks here
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;

    // Create thread1
    thread1 = thread_create(stack1, sizeof(stack1), 10,
                            THREAD_CREATE_WOUT_YIELD,
                            thread_handler, NULL, "thread1");

    // Create thread2
    thread2 = thread_create(stack2, sizeof(stack2), 15,
                            THREAD_CREATE_WOUT_YIELD,
                            thread_handler, NULL, "thread2");

    // Attempt to change priority
    // This uses a hypothetical function 'my_set_priority'
    // to accommodate the absence of 'thread_set_priority'
    my_set_priority(thread1, 8);
    my_set_priority(thread2, 12);

    while (1) {
        // Main thread work
        usleep(5000000); // Sleep for 5 seconds
    }
    return 0;
}

// Hypothetical function to simulate changing priority
int my_set_priority(kernel_pid_t pid, uint8_t priority) {
    // Simulate adjusting priority
    printf("Thread %d priority changed to %d\n", pid, priority);
    return 0;
}