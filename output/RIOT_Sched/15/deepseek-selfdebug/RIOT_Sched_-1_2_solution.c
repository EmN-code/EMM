
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <log.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + 512)

kernel_pid_t low_priority_thread_pid;
kernel_pid_t high_priority_thread_pid;

void *low_priority_thread(void *arg)
{
    (void)arg;
    while (1) {
        printf("Low priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *high_priority_thread(void *arg)
{
    (void)arg;
    while (1) {
        printf("High priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    log_init();

    char low_stack[THREAD_STACKSIZE_MAIN];
    char high_stack[THREAD_STACKSIZE_MAIN];

    low_priority_thread_pid = thread_create(low_stack, sizeof(low_stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            low_priority_thread, NULL, "low");

    high_priority_thread_pid = thread_create(high_stack, sizeof(high_stack),
                                             THREAD_PRIORITY_MAIN - 2,
                                             THREAD_CREATE_STACKTEST,
                                             high_priority_thread, NULL, "high");

    xtimer_sleep(10);

    printf("Changing priority of low priority thread to higher\n");
    sched_change_priority(low_priority_thread_pid, THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(10);

    printf("Changing priority of high priority thread to lower\n");
    sched_change_priority(high_priority_thread_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
