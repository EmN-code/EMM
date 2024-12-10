
#include "mbox.h"
#include "evtimer_msg.h"
#include "net/sock/udp.h"
#include "timex.h"
#include "architecture.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "thread.h"
#include "periph/i2c.h"
#include "ztimer.h"
#include "event/thread.h"
#include "mma8x5x.h"
#include "net/emcute.h"
#include "evtimer_mbox.h"
#include "freertos/FreeRTOS.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "sys/socket.h"
#include "sched.h"
#include "freertos/task.h"
#include "net/gcoap.h"
#include "event/periodic_callback.h"
#include "assert.h"
#include "net/sock/util.h"

#define THREAD_STACKSIZE_DEFAULT (512)

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
