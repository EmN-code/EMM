
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "pthread.h"

#include "event/thread.h"
#include "event/periodic.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"

#define TIMER_INTERVAL 1 /* interval in seconds */

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[16];

static void callback(event_t *event)
{
    /* This function will be called every TIMER_INTERVAL seconds */
    puts("Callback function called.");
}

void *evt_thread(void *arg) {
    event_periodic_t periodic_timer;
    (void)arg;

    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    /* Initialize the periodic timer */
    event_periodic_init(&periodic_timer, EVENT_PRIO_LOWEST, TIMER_INTERVAL * US_PER_SEC, callback, NULL);
    event_periodic_start(&periodic_timer);

    /* Enter infinite loop */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }
    return NULL;
}

int main(void)
{
    puts("Event-based timer application started.");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  evt_thread, NULL, "evt_thread");

    return 0;
}