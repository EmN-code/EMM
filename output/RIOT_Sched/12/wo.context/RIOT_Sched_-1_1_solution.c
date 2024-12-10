#include <thread_config.h>
#include <stdio.h>
#include <thread_arch.h>

void *thread1(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread2(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    thread_t my_thread1;
    thread_t my_thread2;
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    printf("Starting threads\n");

    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                  THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    printf("Threads started\n");

    // Change priority of thread1
    thread_set_priority(thread_getpid_of(&my_thread1), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread 1\n");

    // Let's just simulate a loop here as threads are running
    while (1) {
        
    }

    return 0;
}