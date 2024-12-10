
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/timer.h"
#include "fmt.h"

#define MAIN_QUEUE_SIZE     (8)
#define TIMER_INTERVAL      (1000000U)  /* 1 second */

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static msg_t mbox_queue[MAIN_QUEUE_SIZE];

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = (uint32_t)arg;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the mailbox */
    mbox_init(&mbox, mbox_queue, MAIN_QUEUE_SIZE);

    /* Initialize the timer */
    timer_init(TIMER_DEV(0), TIMER_SPEED, timer_callback, NULL);

    /* Start the timer */
    timer_set_periodic(TIMER_DEV(0), 0, TIMER_INTERVAL, TIMER_CB_UNDEF);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
