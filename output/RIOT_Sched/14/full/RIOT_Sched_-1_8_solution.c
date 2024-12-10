#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "thread_arch.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "xtimer.h"
#include "sys/socket.h"
#include "timex.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)
#define MAIN_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define SECOND_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void * thread1_handler(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void * thread2_handler(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    puts("RIOT Basic Thread Example\n");

    // Create threads
    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1), MAIN_PRIORITY, 0, thread1_handler, NULL, "Thread 1");
    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2), SECOND_PRIORITY, 0, thread2_handler, NULL, "Thread 2");

    if (thread1_pid <= KERNEL_PID_UNDEF || thread2_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create threads");
        return 1;
    }

    // Simulating changing thread priority at runtime
    sched_change_priority(thread_get(thread1_pid), SECOND_PRIORITY);
    sched_change_priority(thread_get(thread2_pid), MAIN_PRIORITY);

    puts("Threads created and priorities changed\n");

    while (1) {
        // Continuously check network connectivity and handle exceptions
        int rc;
        while (1) {
            rc = NetworkConnect(&network, "192.168.1.1", 1883);
            if (rc == 0) {
                printf("Connected successfully\n");
                break;
            } else {
                printf("Connection failed, retrying...\n");
                xtimer_sleep(1);
                continue;
            }
        }
        // Other operations can be added here as needed
    }
    return 0;
}