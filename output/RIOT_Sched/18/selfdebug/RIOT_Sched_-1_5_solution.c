#include <thread.h>

// Function to demonstrate changing priority
void *thread1(void *arg) {
    // Busy wait
    for (;;) {
        // Perform some repetitive task
    }
    return NULL;
}

// Function to demonstrate changing priority
void *thread2(void *arg) {
    // Busy wait
    for (;;) {
        // Perform some repetitive task
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t pid1;

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");
    
    // Create the second thread with lower priority
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                      THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    // Change the priority of the first thread
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);

    // Sleep for an indefinite period (this demonstrates sleeping
    // and prevents the main function from exiting)
    thread_sleep();

    return 0;
}