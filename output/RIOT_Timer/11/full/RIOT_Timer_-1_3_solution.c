#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static ztimer_t timer;
static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED here
    printf("LED toggled\n");
    // Place hardware-specific LED toggle function here
}

int main(void) {
    ztimer_set(&timer, INTERVAL);
    timer.callback = timer_callback;

    pid_t scheduler_pid = thread_create(scheduler_stack, sizeof(scheduler_stack), SCHED_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, scheduler, NULL, "scheduler");
    msg_t msg;

    while (1) {
        if (ztimer_msg_try_send(&timer, &msg, scheduler_pid)) {
            printf("Timer set and message sent to scheduler\n");
        } else {
            printf("Failed to set timer message\n");
        }
        usleep(INTERVAL);
    }

    return 0;
}