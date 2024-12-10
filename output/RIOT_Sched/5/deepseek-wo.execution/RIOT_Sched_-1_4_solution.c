
#include "mbox.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "architecture.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "mma8x5x.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "sched.h"
#include "freertos/task.h"
#include "net/gcoap.h"
#include "event/periodic_callback.h"
#include "assert.h"

#define THREAD_STACKSIZE_DEFAULT (512)

static char thread1_stack[THREAD_STACKSIZE_DEFAULT];
static char thread2_stack[THREAD_STACKSIZE_DEFAULT];

void thread1_func(void)
{
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
}

void thread2_func(void)
{
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
}

int main(void)
{
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)thread1_func, NULL, "thread1");
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD_PRIORITY_MAIN - 2, 0, (thread_task_func_t)thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread1 to higher\n");
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(5);

    printf("Changing priority of thread2 to higher\n");
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
