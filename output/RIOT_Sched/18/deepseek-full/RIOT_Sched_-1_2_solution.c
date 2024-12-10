
#include "thread.h"
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "ztimer.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define THREAD_STACKSIZE 512
#define THREAD1_PRIO (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIO (THREAD_PRIORITY_MAIN - 2)

char thread1_stack[THREAD_STACKSIZE];
char thread2_stack[THREAD_STACKSIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD1_PRIO, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD2_PRIO, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing thread priorities at runtime\n");

    thread_set_priority(thread1_pid, THREAD2_PRIO);
    thread_set_priority(thread2_pid, THREAD1_PRIO);

    return 0;
}
