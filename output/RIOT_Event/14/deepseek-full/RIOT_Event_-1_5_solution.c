
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define STACK_SIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static kernel_pid_t pid;

void periodic_callback(void)
{
    printf("Periodic callback triggered!\n");
}

void timer_thread(void)
{
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        periodic_callback();
    }
}

int main(void)
{
    pid = thread_create(stack, sizeof(stack), PRIORITY, 0, (thread_task_func_t)timer_thread, NULL, "timer_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}
