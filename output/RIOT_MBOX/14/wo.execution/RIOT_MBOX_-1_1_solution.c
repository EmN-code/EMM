#include <ztimer.h> // Provides timer functions
#include <msg.h> // Provides message passing functions
#include <stdio.h> // Standard I/O functions

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

// This is the callback function called on timer events
static void _callback(void *arg)
{
    msg_t msg;
    msg_send(&msg, (msg_t *)arg);
    printf("Message sent\n");
}

int main(void)
{
    // Initialize message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Setup periodic timer
    ztimer_t timer;
    ztimer_set_msg_timeout(ZTIMER_MSEC, &timer, 1000, msg_receive(&msg), NULL);

    printf("Message box initialized.\n");

    // Start the mbox handler
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received\n");
    }

    return 0;
}