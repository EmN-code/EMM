
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <ztimer.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0; // Dummy value
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize ztimer for periodic events
    ztimer_t timer;
    ztimer_init(&timer);

    // Set the timer to trigger every INTERVAL_MS milliseconds
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL_MS, timer_callback, NULL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message from mbox\n");
    }

    return 0;
}
