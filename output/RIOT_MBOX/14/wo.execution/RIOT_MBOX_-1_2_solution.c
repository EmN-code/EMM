#include <xtimer.h>  // Ensure necessary functions for timing are available
#include <msg.h>     // For message queue functionalities
#include <stdio.h>   // General I/O operations

#define INTERVAL (1000000U) // Interval of 1 second for message box

static mbox_t mbox; // Define a message box
static msg_t queue[8]; // Define message box queue

// Callback function to be used with ztimer
static void callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message content
    msg_send(&msg, mbox_recipient_pid()); // Correct function usage
}

int main(void) {
    mbox_init(&mbox, queue, ARRAY_SIZE(queue)); // Initialize the message box
    
    xtimer_init(); // Initialize the timer system
    ztimer_set_timeout(ZTIMER_MSEC, INTERVAL); // Set up timer with correct interval

    msg_t msg;

    // Event loop
    while (1) {
        if(_mbox_get(&mbox, &msg, 1) == 1) { // Non-blocking retrieval from message box
            printf("Received msg with value: %lu\n", msg.content.value);
        }
        xtimer_usleep(INTERVAL); // Sleep for a defined interval
    }
    return 0;
}