
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/timer.h"

#define MSG_QUEUE_SIZE (8)
#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void timer_cb(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = xtimer_now_usec();
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the timer
    timer_init(TIMER_DEV(0), 1000000, timer_cb, NULL);

    // Start the timer
    timer_set_periodic(TIMER_DEV(0), 0, TIMER_INTERVAL, timer_cb, NULL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, 0);
        printf("Received message with timestamp: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
