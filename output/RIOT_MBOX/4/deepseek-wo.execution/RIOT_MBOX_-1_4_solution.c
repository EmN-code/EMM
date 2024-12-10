
#include <evtimer_msg.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <mbox.h>
#include <stdio.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer with the mailbox
    evtimer_init_mbox(&mbox);

    // Create a message to send
    msg_t msg;
    msg.content.value = 42; // Example value

    // Add a message to the event timer with a delay
    evtimer_add_msg(&mbox, &msg, ZTIMER_TICKS(xtimer_ticks(1000000))); // 1 second delay

    // Wait for a message to be received
    msg_t received_msg;
    mbox_get(&mbox, &received_msg, 1); // Blocking call

    // Print the received message
    printf("Received message with value: %d\n", (int)received_msg.content.value);

    return 0;
}
