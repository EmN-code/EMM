
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <periph/timer.h> // Include the correct header for timers

#define MSG_QUEUE_SIZE (8)
#define TIMER_INTERVAL (1000000U) // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0; // Example content
    msg_send(&msg, mbox_pid(&mbox));
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Set up the timer
    timer_t timer;
    timer_init(TIMER_DEV(0), TIMER_INTERVAL, timer_callback, NULL);
    timer_start(TIMER_DEV(0));

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }

    return 0;
}
