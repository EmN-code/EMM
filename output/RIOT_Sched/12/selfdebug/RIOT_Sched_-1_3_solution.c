#include <thread.h>
#include <stdlib.h>

#define THREAD_STACKSIZE     (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_1    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2    (THREAD_PRIORITY_MAIN - 2)

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];
kernel_pid_t thread1_pid, thread2_pid;

void *thread_func1(void *arg) {
    (void)arg;
    // Simulate some work
    int i = 0;
    while (1) {
        printf("Thread 1 running with priority %u\n", thread_get_priority(thread1_pid));
        for (volatile int j = 0; j < 100000; j++) {
            i += j;  // Some dummy operation
        }
        thread_yield();  // Yield to other threads
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    // Simulate some work
    int i = 0;
    while (1) {
        printf("Thread 2 running with priority %u\n", thread_get_priority(thread2_pid));
        for (volatile int j = 0; j < 100000; j++) {
            i += j;  // Some dummy operation
        }
        thread_yield();  // Yield to other threads
    }
    return NULL;
}

int main(void)
{
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1,
                                THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2,
                                THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Run some operation that changes the priority
    thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);
    thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN - 4);

    while (1) {
        // Keep the main thread alive
    }

    return 0;
}