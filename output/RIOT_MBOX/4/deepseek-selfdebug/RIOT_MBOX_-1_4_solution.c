
#include <stdio.h>
#include <xtimer.h>
#include <msg.h>
#include <periph/timer.h>

#define TIMER_DEV         TIMER_DEV(0)
#define TIMER_CHAN        0
#define TIMER_INTERVAL    (1000000U)  /* 1 second */

static msg_t msg_queue[8];
static mbox_t mbox;

static void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    /* Initialize the mailbox */
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    /* Initialize the timer */
    timer_init(TIMER_DEV, TIMER_SPEED, timer_callback, NULL);
    timer_set_absolute(TIMER_DEV, TIMER_CHAN, xtimer_now_usec() + TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Event received at %" PRIu32 "\n", xtimer_now_usec());
        timer_set_absolute(TIMER_DEV, TIMER_CHAN, xtimer_now_usec() + TIMER_INTERVAL);
    }

    return 0;
}
