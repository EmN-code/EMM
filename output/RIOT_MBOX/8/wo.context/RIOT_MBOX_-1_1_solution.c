#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define MSG_TYPE_EVENT (0x999) // Arbitrary message type for the event
#define INTERVAL       (2U)    // Interval in seconds for sending messages

// Message buffer
static msg_t msg_queue[8];
static msg_t msg;

void sender_function(void) {
    // Fill the message
    msg.type = MSG_TYPE_EVENT;

    // Send message (non-blocking)
    if (msg_try_send(&msg) != 1) {
        puts("Error: Message send failed");
    }
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, 8);
    puts("Message queue initialized\n");

    // Setup a ztimer event
    ztimer_t timer;
    ztimer_set(ZTIMER_SEC, &timer, INTERVAL, (ztimer_callback_t)sender_function, NULL);

    puts("Started sending events every 2 seconds\n");

    while (1) {
        // Process received message
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_EVENT) {
            puts("Received event message\n");
        }
    }
    return 0;
}