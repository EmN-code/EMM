
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/timer.h"
#include "fmt.h"

#define MAIN_QUEUE_SIZE     (8)
#define TIMER_INTERVAL      (1000000U)  /* 1 second */

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static msg_t mbox_queue[MAIN_QUEUE_SIZE];

static void timer_cb(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;
    mbox_put(&mbox, &msg);
}

static void *sender_thread_func(void *arg)
{
    (void)arg;
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the mailbox */
    mbox_init(&mbox, mbox_queue, MAIN_QUEUE_SIZE);

    /* Create the sender thread */
    thread_t *sender_thread = thread_create(
        thread_stack(2048),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread_func,
        NULL,
        "sender_thread"
    );

    /* Initialize the timer */
    timer_init(TIMER_DEV(0), 1000000, timer_cb, NULL);

    /* Start the timer */
    timer_set(TIMER_DEV(0), 0, TIMER_INTERVAL);

    return 0;
}
