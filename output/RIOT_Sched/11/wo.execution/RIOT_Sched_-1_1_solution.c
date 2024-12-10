#include <thread_arch.h>
#include <thread_config.h>
#include <net/protnum.h>

// Thread stack size
#define THREAD_STACKSIZE 1024

// Function prototypes
void *threadFunc1(void *arg);
void *threadFunc2(void *arg);

int main(void) {
    // Define thread variables
    thread_t thread1, thread2;
    char stack1[THREAD_STACKSIZE];
    char stack2[THREAD_STACKSIZE];

    // Create thread 1
    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                  threadFunc1, NULL, "Thread 1");

    // Create thread 2
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1, 0,
                  threadFunc2, NULL, "Thread 2");

    // Change priority of thread 1 after starting
    sched_change_priority(thread_get(1), THREAD_PRIORITY_MAIN + 2);

    // Placeholder to keep main running
    while(1) {}

    return 0;
}

void *threadFunc1(void *arg) {
    // Thread 1 task
    while(1) {
        // Do some work here
        
    }
    return NULL;
}

void *threadFunc2(void *arg) {
    // Thread 2 task
    while(1) {
        // Do some work here
        
    }
    return NULL;
}