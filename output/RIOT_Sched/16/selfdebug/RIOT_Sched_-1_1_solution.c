#include "thread_arch.h"
#include "thread_config.h"
#include "net/protnum.h"
#include <stdio.h>
#include <stdlib.h>

// Thread definitions
#define STACKSIZE_MAIN_THREAD  (THREAD_STACKSIZE_DEFAULT)
#define STACKSIZE_OTHER_THREAD  (THREAD_STACKSIZE_DEFAULT)
#define MAIN_THREAD_PRIO (THREAD_PRIORITY_MAIN)
#define OTHER_THREAD_PRIO (THREAD_PRIORITY_MAIN + 1)

char main_thread_stack[STACKSIZE_MAIN_THREAD];
char other_thread_stack[STACKSIZE_OTHER_THREAD];

void *main_thread(void *arg)
{
    (void)arg;
    printf("Main thread started\n");
    while (1) {
        printf("It is running!\n");
        thread_yield(); // Yield to other threads
    }
    return NULL;
}

void *other_thread(void *arg)
{
    (void)arg;
    printf("Other thread started\n");
    while (1) {
        printf("I am working too!\n");
        thread_yield(); // Yield to other threads
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t main_thread_pid;
    kernel_pid_t other_thread_pid;

    // Create main thread
    main_thread_pid = thread_create(main_thread_stack, STACKSIZE_MAIN_THREAD,
                                    MAIN_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                                    main_thread, NULL, "main_thread");

    if (main_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating main thread\n");
        return 1;
    }

    // Create other thread
    other_thread_pid = thread_create(other_thread_stack, STACKSIZE_OTHER_THREAD,
                                     OTHER_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                                     other_thread, NULL, "other_thread");

    if (other_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating other thread\n");
        return 1;
    }

    // Let the threads run for some time
    xtimer_sleep(5);

    // Change priority of the threads
    printf("Changing priorities of the threads\n");
    sched_change_priority(main_thread_pid, OTHER_THREAD_PRIO);
    sched_change_priority(other_thread_pid, MAIN_THREAD_PRIO);

    // Show threads running with new priorities
    xtimer_sleep(5);

    printf("Completed Priority Changes!\n");
    return 0;
}