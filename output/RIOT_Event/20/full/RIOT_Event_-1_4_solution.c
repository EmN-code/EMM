#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"

#define SLEEP_TIME_USEC 1000000

static void callback(void *arg) {
    (void)arg;
    printf("Callback triggered every second\n");
}

int main(void) {
    evtimer_msg_event_t event;
    event.evqueue.msg = THREAD_PID_MAIN;  // Correct type for kernel_pid_t
    evtimer_init_msg(&event.evqueue, msg_array, THREAD_PID_MAIN, THREAD_PRIORITY_MAIN - 1); // Correct types provided

    evtimer_msg_t timer;
    evtimer_msg_init(&timer, &event.evqueue, callback);
    evtimer_msg_set_every(&timer, SLEEP_TIME_USEC);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}