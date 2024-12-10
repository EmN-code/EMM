#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/callback.h"

#define INTERVAL (1U * US_PER_SEC)

static void my_event_callback(void *arg) {
    (void)arg;
    puts("Callback triggered every second!");
}

static evtimer_msg_event_t my_event;
static evtimer_t my_evtimer;
static msg_t my_msg_queue[8];

int main(void) {
    /* Initialize message queue for inter-process communications */
    msg_init_queue(my_msg_queue, ARRAY_SIZE(my_msg_queue));

    /* Initialize the evtimer for messages */
    evtimer_init_mbox(&my_evtimer);

    /* Set the timer callback function */
    my_event.event.callback = my_event_callback;

    /* Run event callback every second */
    evtimer_add_msg(&my_evtimer, &my_event, thread_getpid());

    /* Start the timer with initial interval */
    xtimer_periodic_init(&my_timer, xtimer_now() + INTERVAL);

    while (1) {
        /* Here we run the timer loop */
        evtimer_periodic_sleep(&my_timer, INTERVAL);
    }

    return 0;
}