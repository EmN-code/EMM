#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN_DEFAULT - 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 Running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 Running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(
        stack1,
        sizeof(stack1),
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        thread_func1,
        NULL,
        "thread1"
    );

    kernel_pid_t pid2 = thread_create(
        stack2,
        sizeof(stack2),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread_func2,
        NULL,
        "thread2"
    );

    // Change priority of threads after creation
    xtimer_sleep(5); // Wait for 5 seconds before changing priorities

    // Disable interrupts before changing priorities
    irq_disable();
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 3);
    // Re-enable interrupts
    irq_enable();

    return 0;
}