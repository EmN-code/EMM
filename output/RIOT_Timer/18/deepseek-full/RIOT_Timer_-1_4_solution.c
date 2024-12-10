
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static kernel_pid_t pid;
static msg_t msg_queue[8];

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;  // Custom message type
    msg_send(&msg, pid);
}

int main(void)
{
    pid = thread_getpid();
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == 1) {
            // Toggle LED logic here
            printf("LED toggled\n");
        }
    }

    return 0;
}
