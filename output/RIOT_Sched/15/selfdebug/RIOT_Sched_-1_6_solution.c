// The code starts two threads and changes the priority of the threads at runtime. Proper headers and function declarations are included.

#include <thread_config.h>
#include <thread.h>

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func(void *arg) {
    (void)arg;   // To avoid unused parameter warning
    // Thread task
    while (1) {}
    return NULL;
}

int main(void) {
    // Create two threads
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, NULL, "thread2");

    // Change priority of the threads
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);
    
    // Main task
    while (1) {}
    return 0;
}