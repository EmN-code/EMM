
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <event/callback.h>
#include <event/periodic.h>
#include <ztimer.h>

#define MSG_QUEUE_SIZE     (8)
#define INTERVAL           (1000000U)  /* 1 second in microseconds */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the mailbox */
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the periodic callback event */
    event_periodic_t periodic;
    event_callback_t event;
    event_periodic_callback_t periodic_callback;

    event_periodic_init(&periodic, ZTIMER_USEC, event_handler, NULL);
    event_callback_init(&event, &periodic, &mbox, event_handler, NULL);
    event_periodic_callback_init(&periodic_callback, &periodic, &event);

    /* Start the periodic callback event */
    event_periodic_callback_start(&periodic_callback, INTERVAL);

    /* Main loop */
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Message received!\n");
        }
    }

    return 0;
}
