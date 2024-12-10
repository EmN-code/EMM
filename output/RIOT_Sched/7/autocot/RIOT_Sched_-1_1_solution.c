#include <thread_arch.h>    // Necessary for thread architecture
#include <thread_config.h>  // Configuration header for thread priority

#include <stdio.h>  // Standard Input Output header

#define THREAD_STACKSIZE_MAIN 1024 // Stack size for main thread
#define THREAD_PRIORITY_MAIN 5     // Priority of main thread

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

// Function signature for thread servers - it can be customized further
void *thread_handler_one(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_handler_two(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid;
    kernel_pid_t thread_two_pid;

    thread_one_pid = thread_create(stack_one, sizeof(stack_one),
                            THREAD_PRIORITY_MAIN + 1, 0, thread_handler_one, NULL, "thread_one");
    
    thread_two_pid = thread_create(stack_two, sizeof(stack_two),
                            THREAD_PRIORITY_MAIN + 2, 0, thread_handler_two, NULL, "thread_two");

    // Change priority of a thread at runtime
    thread_set_priority(thread_one_pid, THREAD_PRIORITY_MAIN + 2);
    thread_set_priority(thread_two_pid, THREAD_PRIORITY_MAIN + 1);

    // Main thread should stay alive
    while (1) {
        // Keeping main thread alive
    }

    return 0;
}