#include <xtimer.h>
#include <thread.h>
#include <sched.h>
#include <stdio.h>

// Definition of thread stack sizes
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

// Thread function declarations
void *thread_a(void *arg);
void *thread_b(void *arg);

// Global thread stack variables
char stack_a[THREAD_STACKSIZE];
char stack_b[THREAD_STACKSIZE];

// Global thread priority variables
int priority_a = THREAD_PRIORITY_MAIN - 1;
int priority_b = THREAD_PRIORITY_MAIN - 2;

int main(void) {
    // Create first thread
    kernel_pid_t pid_a = thread_create(stack_a, sizeof(stack_a), priority_a, 0, thread_a, NULL, "thread_a");
    printf("Thread A started with PID: %d\n", pid_a);

    // Create second thread
    kernel_pid_t pid_b = thread_create(stack_b, sizeof(stack_b), priority_b, 0, thread_b, NULL, "thread_b");
    printf("Thread B started with PID: %d\n", pid_b);
    
    // Let's change the priority of thread A to a lower priority
    uint8_t new_priority = priority_b - 1; // Make thread A have lower priority
    sched_change_priority(thread_get(pid_a), new_priority);
    printf("Priority of Thread A changed to: %d\n", new_priority);

    // The main thread has nothing else to do, halt it
    thread_sleep();

    return 0;
}

void *thread_a(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread A running\n");
        xtimer_sleep(2);
    }

    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread B running\n");
        xtimer_sleep(3);
    }

    return NULL;
}