#include <thread.h>
#include <sched.h>
#include <stdio.h>

#define THREAD_STACKSIZE_LARGE   (THREAD_STACKSIZE_DEFAULT)

char stack[THREAD_STACKSIZE_LARGE];

void *thread_function(void *arg) {
    printf("Thread %d running\n", thread_getpid());
    while (1) {
        // Simulate doing some work
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    thread_t *thread;
    
    // Create a thread
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, thread_function, NULL, "thread");
    
    // Retrieve the thread struct using thread_get
    thread = thread_get(thread_pid);
    
    if (thread != NULL) {
        // Change the thread's priority
        printf("Changing the thread priority\n");
        sched_change_priority(thread, THREAD_PRIORITY_MAIN - 2);
    }
    else {
        printf("Failed to get the thread\n");
    }

    // Start a second thread
    kernel_pid_t thread_pid2 = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                             THREAD_CREATE_STACKTEST, thread_function, NULL, "thread2");

    // Similar priority change for the second thread
    thread = thread_get(thread_pid2);
    if (thread != NULL) {
        printf("Changing the second thread priority\n");
        sched_change_priority(thread, THREAD_PRIORITY_MAIN - 3);
    }
    else {
        printf("Failed to get the second thread\n");
    }
    
    return 0;
}