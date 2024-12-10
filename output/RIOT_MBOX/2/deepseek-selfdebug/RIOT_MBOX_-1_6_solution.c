
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <event/periodic.h>

#define INTERVAL (1000000U)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;

void event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Create and initialize the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_USEC, &mbox, event_handler, NULL);

    // Start the periodic callback event
    event_periodic_callback_start(&event, INTERVAL);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Message received!\n");
        }
    }

    return 0;
}
