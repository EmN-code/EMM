#include <thread_config.h>
#include <thread_arch.h>

void *thread_one(void *arg) {
    while (1) {
        // Simulate some work in thread one
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        // Simulate some work in thread two
    }
    return NULL;
}

int main(void) {
    // Initialize the threads
    thread_t thread1, thread2;
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid1, pid2;

    // Create the thread, running `thread_one` with a priority of 5
    pid1 = thread_create(stack1, sizeof(stack1), 5, THREAD_CREATE_STACKTEST,
                         thread_one, NULL, "thread_one");

    // Create the thread, running `thread_two` with a priority of 8
    pid2 = thread_create(stack2, sizeof(stack2), 8, THREAD_CREATE_STACKTEST,
                         thread_two, NULL, "thread_two");

    // Modify the priority of the threads at runtime
    thread_set_priority(pid1, 10);  // Adjust priority for thread one
    thread_set_priority(pid2, 4);   // Adjust priority for thread two

    // Threads now running with new priorities
    return 0;
}