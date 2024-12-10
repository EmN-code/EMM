#include <xtimer.h>  // ztimer and other timer related functions
#include <unistd.h>  // standard symbolic constants and types
#include <timex.h>    // timex type for time
#include <stdlib.h>   // standard library functions
#include <stdatomic.h>// atomic operations
#include <stdbool.h>  // to use bool data type
#include <stddef.h>   // standard type definitions
#include <stdio.h>    // standard input/output definitions
#include <msg.h>      // message API for message handling
#include <mma8x5x_regs.h>
#include <mbox.h>     // mailbox API for message queue
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <evtimer.h>  // event timer API
#include <ztimer.h>   
#include <time_units.h>  // for use with time units

#define MAIN_QUEUE_SIZE (4)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];


#define DELAY 1000000  // 1 second delay in microseconds

evtimer_t evtimer;
evtimer_mbox_event_t mbox_event;
mbox_t mbox;
msg_t msg_queue[MAIN_QUEUE_SIZE];

int main(void) {
    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the mailbox for communication
    mbox_init(&mbox, msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the event timer to handle mailbox events
    evtimer_init_mbox(&evtimer);

    // Set event attributes
    mbox_event.event.period = DELAY;

    // Add mailbox event to the event timer
    evtimer_add_mbox(&evtimer, &mbox_event, &mbox);

    // Event loop to receive message box messages
    while (1) {
        msg_t msg;
        // Blocks until a message is received
        mbox_get(&mbox, &msg);

        // Handle the received message (print a statement for demonstration)
        printf("Received message with type: %d, value: %ld\n", msg.type, msg.content.value);
    }

    return 0;
}