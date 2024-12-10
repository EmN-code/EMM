#include "thread_arch.h"
#include "thread_config.h"
#include "stdio.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread1(void *arg) {
    printf("Thread 1 running\n");
    while (1) {
        // Perform thread tasks
    }
    return NULL;
}

void *thread2(void *arg) {
    printf("Thread 2 running\n");
    while (1) {
        // Perform thread tasks
    }
    return NULL;
}

int main(void) {
    thread_t thread1_handle;
    thread_t thread2_handle;

    printf("Starting threads...\n");

    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1, NULL, "thread1");
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2, NULL, "thread2");

   // Change priority of thread1
    thread_set_priority(thread_getpid_of(&thread1_handle), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread1\n");

    return 0;
}