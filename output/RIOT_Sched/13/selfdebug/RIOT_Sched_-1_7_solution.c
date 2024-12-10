#include "thread.h"

#define THREAD_STACKSIZE_MAIN   THREAD_STACKSIZE_DEFAULT
#define MY_THREAD_PRIORITY      (THREAD_PRIORITY_MAIN - 1)

void *thread1_handler(void *arg) {
    (void)arg;
    // Thread 1 logic here
    while(1) {
        // Simulate work
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    // Thread 2 logic here
    while(1) {
        // Simulate work
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1),
                                         MY_THREAD_PRIORITY, CREATE_STACKTEST,
                                         thread1_handler, NULL, "thread1");
    
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2),
                                         MY_THREAD_PRIORITY, CREATE_STACKTEST,
                                         thread2_handler, NULL, "thread2");
    
    // Change the priority of thread1 to a higher priority
    thread_get(thread1);
    sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN - 2);
    
    return 0;
}