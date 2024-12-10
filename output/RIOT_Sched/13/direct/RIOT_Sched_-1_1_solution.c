#include <thread.h>
#include <stdio.h>

// Define stack size for threads
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)

// Thread stacks
char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

// Thread arguments
static const char *thread1_name = "thread1";
static const char *thread2_name = "thread2";

// Thread function for thread1
void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 started with priority %d \n", thread_get_priority(thread_getpid()));
    while (1) {
        // Thread 1 performs its task
        puts("Thread 1 is running");
        xtimer_sleep(1);
    }
    return NULL;
}

// Thread function for thread2
void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 started with priority %d \n", thread_get_priority(thread_getpid()));
    while (1) {
        // Thread 2 performs its task
        puts("Thread 2 is running");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    printf("Starting threads with initial priorities\n");

    // Create thread1 with priority 6
    thread_create(stack1, sizeof(stack1), 6, THREAD_CREATE_STACKTEST, thread1_func, (void *)thread1_name, "thread1");
    
    // Create thread2 with priority 8
    thread_create(stack2, sizeof(stack2), 8, THREAD_CREATE_STACKTEST, thread2_func, (void *)thread2_name, "thread2");

    xtimer_sleep(5);

    printf("Changing priorities of threads\n");

    // Change priority of thread1 to 8
    thread_set_priority(thread_getpid_from_name(thread1_name), 8);

    // Change priority of thread2 to 6
    thread_set_priority(thread_getpid_from_name(thread2_name), 6);

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}