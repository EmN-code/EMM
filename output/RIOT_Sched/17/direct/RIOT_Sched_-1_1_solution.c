#include <thread.h>
#include <stdio.h>
#include <stdlib.h>

#define PRIORITY_MAIN 5  // Priority of main thread (0 highest, 255 lowest)
#define PRIORITY_THREAD_A 4  // Initial priority of thread A
#define PRIORITY_THREAD_B 6  // Initial priority of thread B

char stack_a[THREAD_STACKSIZE_MAIN];  // Stack size for thread A
char stack_b[THREAD_STACKSIZE_MAIN];  // Stack size for thread B

void *thread_a(void *arg) {
    (void)arg;
    printf("Thread A is running with initial priority %d\n", thread_get_priority(thread_getpid()));
    thread_set_priority(thread_getpid(), PRIORITY_THREAD_B);
    printf("Thread A changed its priority to %d\n", thread_get_priority(thread_getpid()));
    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    printf("Thread B is running with initial priority %d\n", thread_get_priority(thread_getpid()));
    thread_set_priority(thread_getpid(), PRIORITY_THREAD_A);
    printf("Thread B changed its priority to %d\n", thread_get_priority(thread_getpid()));
    return NULL;
}

int main(void) {
    thread_create(stack_a, sizeof(stack_a), PRIORITY_THREAD_A, THREAD_CREATE_STACKTEST, thread_a, NULL, "thread_a");
    thread_create(stack_b, sizeof(stack_b), PRIORITY_THREAD_B, THREAD_CREATE_STACKTEST, thread_b, NULL, "thread_b");
    
    while (1) {
        // Main thread loop doing other processing
        printf("Main thread running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return 0;
}