#include <thread.h>
#include <stdio.h>

// Define stack size for the threads
#define STACK_SIZE_THREAD1 (THREAD_STACKSIZE_DEFAULT)
#define STACK_SIZE_THREAD2 (THREAD_STACKSIZE_DEFAULT)

// Define priorities
#define PRIORITY_THREAD1 (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD2 (THREAD_PRIORITY_MAIN - 2)

char stack_thread1[STACK_SIZE_THREAD1];
char stack_thread2[STACK_SIZE_THREAD2];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running with priority %d\n", PRIORITY_THREAD1);
        thread_yield();
    }
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running with priority %d\n", PRIORITY_THREAD2);
        thread_yield();
    }
    return NULL;
}

int main(void) {
    puts("Starting multi-threading example on ESP32\n");

    // Start the threads
    thread1_pid = thread_create(stack_thread1, sizeof(stack_thread1),
                                PRIORITY_THREAD1, 0, thread1, NULL, "thread1");

    thread2_pid = thread_create(stack_thread2, sizeof(stack_thread2),
                                PRIORITY_THREAD2, 0, thread2, NULL, "thread2");

    // Change priority of threads at runtime
    printf("Changing priorities...\n");
    thread_set_priority(thread1_pid, PRIORITY_THREAD2);
    thread_set_priority(thread2_pid, PRIORITY_THREAD1);

    // Let the threads run indefinitely
    while (1) {
        thread_yield();
    }

    return 0;
}