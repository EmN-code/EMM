
#include "thread.h"
#include "msg.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 2)

char thread_stack[THREAD_STACKSIZE_MAIN];

void *thread_low_priority(void *arg)
{
    (void)arg;
    while (1) {
        // Low priority thread logic
        thread_yield();
    }
    return NULL;
}

void *thread_high_priority(void *arg)
{
    (void)arg;
    while (1) {
        // High priority thread logic
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    log_init();

    kernel_pid_t low_pid = thread_create(thread_stack, sizeof(thread_stack),
                                         THREAD_PRIORITY_LOW, THREAD_CREATE_SLEEPING,
                                         thread_low_priority, NULL, "low_priority");

    kernel_pid_t high_pid = thread_create(thread_stack, sizeof(thread_stack),
                                          THREAD_PRIORITY_HIGH, THREAD_CREATE_SLEEPING,
                                          thread_high_priority, NULL, "high_priority");

    thread_wakeup(low_pid);
    thread_wakeup(high_pid);

    // Change priority at runtime
    sched_change_priority(thread_get(low_pid), THREAD_PRIORITY_HIGH);
    sched_change_priority(thread_get(high_pid), THREAD_PRIORITY_LOW);

    return 0;
}
