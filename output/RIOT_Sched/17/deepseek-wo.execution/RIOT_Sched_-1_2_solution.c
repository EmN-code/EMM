
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t thread_high_pid;
kernel_pid_t thread_low_pid;

void *thread_high(void *arg)
{
    (void)arg;
    while (1) {
        printf("High priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    while (1) {
        printf("Low priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    char stack_high[THREAD_STACKSIZE];
    char stack_low[THREAD_STACKSIZE];

    thread_high_pid = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_high, NULL, "high");
    thread_low_pid = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_low, NULL, "low");

    xtimer_sleep(10);

    printf("Changing priority of high thread to low\n");
    sched_change_priority(thread_high_pid, PRIORITY_LOW);

    xtimer_sleep(10);

    printf("Changing priority of low thread to high\n");
    sched_change_priority(thread_low_pid, PRIORITY_HIGH);

    return 0;
}
