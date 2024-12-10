
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;
    msg_t msg_queue[8];
    mbox_t mbox = MBOX_INIT(msg_queue, 8);

    // Initialize the periodic event timer
    xtimer_init(&timer);
    xtimer_set_msg(&timer, TIMER_INTERVAL, &msg, thread_getpid());

    while (1) {
        msg_receive(&msg);
        event_periodic_callback(NULL);
        xtimer_set_msg(&timer, TIMER_INTERVAL, &msg, thread_getpid());
    }

    return 0;
}
