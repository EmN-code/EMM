#include <thread.h>
#include <sched.h>

// Thread stack sizes
#define STACKSIZE_THREAD1 (THREAD_STACKSIZE_DEFAULT)
#define STACKSIZE_THREAD2 (THREAD_STACKSIZE_DEFAULT)

// Thread priorities
#define PRIORITY_THREAD1 (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD2 (THREAD_PRIORITY_MAIN - 2)

char stack1[STACKSIZE_THREAD1];
char stack2[STACKSIZE_THREAD2];

// Function for thread1
void *thread_func1(void *arg) {
    (void)arg;
    // Perform some task
    while(1) {}
    return NULL;
}

// Function for thread2
void *thread_func2(void *arg) {
    (void)arg;
    // Perform some task
    while(1) {}
    return NULL;
}


int main(void) {
    kernel_pid_t pid_thread1, pid_thread2;

    // Create thread1
    pid_thread1 = thread_create(stack1, STACKSIZE_THREAD1, PRIORITY_THREAD1, 0, thread_func1, NULL, "thread1");

    // Create thread2
    pid_thread2 = thread_create(stack2, STACKSIZE_THREAD2, PRIORITY_THREAD2, 0, thread_func2, NULL, "thread2");

    // Change priority of thread1
    sched_change_priority(thread_get(pid_thread1), THREAD_PRIORITY_MAIN);

    // Change priority of thread2
    sched_change_priority(thread_get(pid_thread2), THREAD_PRIORITY_MAIN);

    return 0;
}
