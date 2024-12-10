
#include "esp_system.h"
#include "thread.h"
#include "xtimer.h"
#include "mutex.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "time_units.h"
#include "timex.h"
#include "kernel_defines.h"
#include "msg.h"
#include "unistd.h"
#include "ztimer.h"

#define STACKSIZE (1024)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN - 2)

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];
static kernel_pid_t pid1, pid2;
static mutex_t priority_mutex = MUTEX_INIT;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void swap_priorities(void)
{
    mutex_lock(&priority_mutex);
    uint8_t prio1 = thread_get_priority(pid1);
    uint8_t prio2 = thread_get_priority(pid2);

    if (prio1 != PRIORITY_LOW) {
        thread_set_priority(pid1, PRIORITY_LOW);
        thread_set_priority(pid2, PRIORITY_HIGH);
        printf("Priorities swapped: Thread 1 -> LOW, Thread 2 -> HIGH\n");
    } else {
        thread_set_priority(pid1, PRIORITY_HIGH);
        thread_set_priority(pid2, PRIORITY_LOW);
        printf("Priorities swapped: Thread 1 -> HIGH, Thread 2 -> LOW\n");
    }
    mutex_unlock(&priority_mutex);
}

int main(void)
{
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    while (1) {
        xtimer_sleep(5);
        swap_priorities();
    }

    return 0;
}
