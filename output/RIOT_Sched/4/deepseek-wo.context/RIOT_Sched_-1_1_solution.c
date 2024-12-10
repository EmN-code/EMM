
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char thread_stack_high[THREAD_STACKSIZE];
char thread_stack_low[THREAD_STACKSIZE];

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    printf("High priority thread started\n");

    while (1) {
        printf("High priority thread running\n");
        xtimer_sleep(2);
    }

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    printf("Low priority thread started\n");

    while (1) {
        printf("Low priority thread running\n");
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void)
{
    printf("Main thread started\n");

    // Create high priority thread
    pid_high = thread_create(thread_stack_high, sizeof(thread_stack_high),
                             THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
                             thread_high, NULL, "thread_high");

    // Create low priority thread
    pid_low = thread_create(thread_stack_low, sizeof(thread_stack_low),
                            THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST,
                            thread_low, NULL, "thread_low");

    // Change priority of low priority thread to high
    thread_change_priority(pid_low, THREAD_PRIORITY_HIGH);

    // Change priority of high priority thread to low
    thread_change_priority(pid_high, THREAD_PRIORITY_LOW);

    printf("Main thread finished\n");

    return 0;
}
