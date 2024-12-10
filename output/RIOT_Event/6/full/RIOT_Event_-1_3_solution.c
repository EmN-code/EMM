#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "event/periodic.h"

#define PERIOD (1U * US_PER_SEC)  // period of one second

char stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Callback triggered!\n");  // This function will be called every second
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    ztimer_t mytimer;
    mytimer.callback = thread_handler;
    mytimer.arg = NULL;
    ztimer_periodic_start(&mytimer, ZTIMER_MSEC, PERIOD);

    // Start a new thread with the periodic task
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_handler, NULL, "periodic_thread");

    while (1) {
        msg_t msg;
        if (msg_receive(&msg) < 0) {
            /* Error: failed to receive message */
        } else {
            /* Handle message */
        }
    }

    return 0;
}