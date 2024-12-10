#include "thread_arch.h"
#include "thread_config.h"

// Thread stack size
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

// Thread priorities
#define PRIORITY_THREAD_1 (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD_2 (THREAD_PRIORITY_MAIN - 2)

// Thread stack declarations
char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

// Placeholder thread function
void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread running with priority: %u\n", thread_get_priority(thread_getpid()));
        thread_yield();
    }
    return NULL;
}

int main(void) {
    // Create two threads
    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY_THREAD_1, THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY_THREAD_2, THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread2");

    // Allow some time for threads to execute
    xtimer_sleep(5);
    
    // Change priorities of the threads at runtime
    sched_change_priority(thread1_pid, PRIORITY_THREAD_2);
    sched_change_priority(thread2_pid, PRIORITY_THREAD_1);

    // Main thread looping to keep the program running
    while (1) {
        printf("Main thread\n");
        xtimer_sleep(2);
    }
    return 0;
}