
#include <stdio.h>
#include <stdint.h>
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "ztimer.h"

#define QUEUE_SIZE (16)
#define INTERVAL (1000000U)  // 1 second in microseconds

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the timer
    ztimer_t timer;
    ztimer_init(&timer, ZTIMER_USEC, timer_callback, NULL);
    ztimer_set(&timer, INTERVAL);

    // Main loop to receive messages from the mailbox
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, 1);  // Blocking call to get a message
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
