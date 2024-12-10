#include <ztimer.h>
#include <msg.h>
#include <embUnit.h>

#define TIMER_INTERVAL 1000000U  // 1 second in microseconds

static msg_t msg_queue[8];  // Create a message queue with a size of 8
static ztimer_t timer;      // Timer instance
static mbox_t mbox;         // Message box instance

// Timer callback to send a message
static void sender_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;  // Define a message type
    msg_send(&msg, mbox);  // Send message to the mbox
    ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL, mbox, &msg);  // Reset timer
}

// Receiver function to handle messages
static void receiver(void) {
    msg_t msg;
    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {  // Blocking wait for incoming message
            if (msg.type == 1) {
                puts("Received message from timer!");
            }
        }
    }
}

int main(void) {
    // Initialize message box
    mbox_init(&mbox, msg_queue, 8);
    puts("Message box initialized.");

    // Initialize timer and set the first callback
    ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL, mbox, NULL);
    ztimer_callback_fn_t sender_cb;
    sender_cb = sender_callback;
    ztimer_set_cb(&timer, TIMER_INTERVAL, sender_cb, NULL);

    // Run receiver function
    receiver();

    return 0;
}