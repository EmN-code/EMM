
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "stdio.h"  // Include stdio.h for printf

#define THREAD_STACKSIZE_DEFAULT (512)  // Define stack size directly
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)  // Define priority directly

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid1, pid2;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        thread_sleep(1000000);  // Corrected thread_sleep usage
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        thread_sleep(1000000);  // Corrected thread_sleep usage
    }
    return NULL;
}

int main(void)
{
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(5);  // Sleep for 5 seconds

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
