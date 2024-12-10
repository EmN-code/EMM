
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <event/periodic.h>
#include <mbox.h>

#define MSG_QUEUE_SIZE     (8)
#define EVENT_INTERVAL     (1000000U)  /* 1 second */

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

    /* Create a periodic event callback */
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_USEC, &mbox, event_callback, (void *)1234);
    event_periodic_callback_start(&event, EVENT_INTERVAL);

    /* Main loop to handle messages */
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIuPTR "\n", msg.content.value);
    }

    return 0;
}
