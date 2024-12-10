
#include "thread.h"
#include "msg.h"
#include "timer.h"
#include "ztimer.h"
#include "event.h"
#include "mbox.h"

#define MAIN_QUEUE_SIZE     (8)
#define TIMER_INTERVAL      (1000000U)  /* 1 second */

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static msg_t mbox_queue[MAIN_QUEUE_SIZE];

static void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  /* Example value */
    mbox_put(&mbox, &msg);
}

static void event_loop(void)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        /* Handle the received message */
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the mailbox */
    mbox_init(&mbox, mbox_queue, MAIN_QUEUE_SIZE);

    /* Create a timer */
    ztimer_t timer;
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL, timer_callback, NULL);

    /* Start the event loop */
    event_loop();

    return 0;
}
