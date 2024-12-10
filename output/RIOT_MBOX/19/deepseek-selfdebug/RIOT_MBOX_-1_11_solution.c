
#include "evtimer.h"
#include "msg.h"
#include "thread.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define EVENT_INTERVAL      (1 * US_PER_SEC)  /* 1 second interval */

static evtimer_t evtimer;
static evtimer_msg_event_t event;

void event_callback(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the event timer */
    evtimer_init(&evtimer);

    /* Initialize the event */
    event.event.offset = 0;
    event.event.callback = event_callback;
    event.event.arg = NULL;
    event.msg.sender_pid = thread_getpid();
    event.msg.type = 0;

    /* Add the event to the event timer */
    evtimer_add_msg(&evtimer, &event, EVENT_INTERVAL);

    /* Main loop */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        /* Handle the message if needed */
    }

    return 0;
}
