
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "event.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE (16)
#define INTERVAL (1000000U)  /* 1 second interval */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void event_callback(void *arg)
{
    msg_t msg;
    msg.content.value = (uintptr_t)arg;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the message box */
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the periodic event callback */
    ztimer_periodic_callback_t periodic_event;
    ztimer_periodic_callback_init(&periodic_event, ZTIMER_USEC, event_callback, (void *)1234);

    /* Set the interval for the periodic event */
    ztimer_periodic_callback_set_interval(&periodic_event, INTERVAL);

    /* Start the periodic event */
    ztimer_periodic_callback_start(&periodic_event);

    /* Main loop to handle messages */
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIuPTR "\n", msg.content.value);
    }

    return 0;
}
