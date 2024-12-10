
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "evtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE 8
#define EVENT_INTERVAL (1 * 1000 * 1000U) /* 1 second */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;
static evtimer_msg_event_t events[1];

void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42; /* Example value */
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    /* Initialize the message box */
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the event timer */
    evtimer_init(&evtimer);

    /* Configure the event */
    events[0].event.offset = EVENT_INTERVAL;
    events[0].event.callback = event_callback;
    events[0].event.arg = NULL;
    events[0].mbox = &mbox;

    /* Add the event to the timer */
    evtimer_add_msg(&evtimer, &events[0]);

    /* Main loop */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
