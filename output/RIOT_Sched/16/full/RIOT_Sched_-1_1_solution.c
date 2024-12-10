#include <thread_arch.h>
#include <thread_config.h>
#include <architecture.h>

// Define stack size and priority constants
#define THREAD_STACKSIZE (1024)
#define THREAD_PRIORITY_MAIN (4)

static char thread_stack_1[THREAD_STACKSIZE];
static char thread_stack_2[THREAD_STACKSIZE];

void *thread_handler(void *arg) {
    // Infinite loop for the thread
    while (1) {
        // Perform task
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid_1;
    kernel_pid_t thread_pid_2;

    // Set priority for the threads
    int priority_1 = THREAD_PRIORITY_MAIN;
    int priority_2 = THREAD_PRIORITY_MAIN + 1;

    // Create thread 1
    thread_pid_1 = thread_create(thread_stack_1, sizeof(thread_stack_1),
                                priority_1, THREAD_CREATE_STACKTEST,
                                thread_handler, NULL, "Thread 1");

    // Create thread 2
    thread_pid_2 = thread_create(thread_stack_2, sizeof(thread_stack_2),
                                priority_2, THREAD_CREATE_STACKTEST,
                                thread_handler, NULL, "Thread 2");

    // Simulate runtime change of priority
    thread_set_priority(thread_pid_1, THREAD_PRIORITY_MAIN + 2);
    thread_set_priority(thread_pid_2, THREAD_PRIORITY_MAIN);

    // Let the main thread sleep
    while (1) {
        thread_yield();
    }

    return 0;
}