
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>

#define STACK_SIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t thread_high, thread_low;

void *thread_high_func(void *arg)
{
    (void)arg;
    printf("High priority thread running\n");
    xtimer_sleep(5);
    printf("High priority thread finished\n");
    return NULL;
}

void *thread_low_func(void *arg)
{
    (void)arg;
    printf("Low priority thread running\n");
    xtimer_sleep(5);
    printf("Low priority thread finished\n");
    return NULL;
}

int main(void)
{
    char stack_high[STACK_SIZE];
    char stack_low[STACK_SIZE];

    thread_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_high_func, NULL, "high_thread");
    thread_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_low_func, NULL, "low_thread");

    xtimer_sleep(2);

    printf("Changing priority of high thread to low\n");
    sched_change_priority(thread_high, PRIORITY_LOW);

    xtimer_sleep(10);

    return 0;
}
