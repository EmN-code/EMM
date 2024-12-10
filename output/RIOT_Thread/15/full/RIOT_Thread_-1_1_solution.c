#include <stdio.h>             // For printf function
#include <stdint.h>            // Standard integer definitions
#include <pthread.h>           // For thread creation and management
#include "msg.h"             // RIOT message API
#include "sched.h"           // Scheduling functions
#include "thread.h"          // Thread management functions
#include "xtimer.h"          // Timer functions for delays

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

// Thread stacks
char high_priority_stack[STACKSIZE];
char low_priority_stack[STACKSIZE];

// Thread functions
void *high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg.content.value = 42;
        msg_send(&msg, (kernel_pid_t)arg);
        printf("High priority thread sent a message!\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received a message with value %u!\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t low_prio_pid;
    
    // Initialize message queue
    msg_init_queue((msg_t *)NULL, 8);
    
    // Create low priority thread
    low_prio_pid = thread_create(low_priority_stack, STACKSIZE, THREAD_PRIORITY_MAIN + 1,
                                 THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_priority");
    
    // Continuously try to create high priority thread with error handling
    while (1) {
        int rc = pthread_create(NULL, NULL, high_priority_thread, (void *)low_prio_pid);
        if (rc != 0) {
            printf("Failed to create high priority thread, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("High priority thread created successfully.\n");
            break;
        }
    }

    while (1) {
        // Main thread does nothing here; all action is in the above threads
    }
    return 0;
}