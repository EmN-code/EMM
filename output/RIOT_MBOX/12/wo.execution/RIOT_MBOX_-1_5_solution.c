#include "xtimer.h"
#include "msg.h"
#include "event.h"
#include "evtimer.h"

#define MAIN_QUEUE_SIZE     (8)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[MAIN_QUEUE_SIZE];

mbox_t mbox;
msg_t queue[8];

static void timer_callback(void *arg) {
    (void) arg;
    printf("Event triggered!\n");
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize mbox */
    mbox_init(&mbox, queue, ARRAY_SIZE(queue));

    /* Initialize the event timer */
    evtimer_t evtimer;
    evtimer_init(&evtimer, timer_callback, NULL);

    /* Setup evtimer to call the callback periodically */
    evtimer_periodic_t periodic_event = {
        .interval = {5, 0}  /* 5 seconds interval */
    };

    /* Add the event to evtimer */
    evtimer_add(&evtimer, &periodic_event.mbox_event, &mbox);

    while (1) {
        /* Wait for messages and invoke event handlers */
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}