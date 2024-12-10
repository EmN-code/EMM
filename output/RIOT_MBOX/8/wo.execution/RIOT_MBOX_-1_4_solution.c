#include <xtimer.h>  // Include for xtimer functionalities, used for intervals
#include <msg.h>     // Include for message queue structures
#include <stdio.h>   // Standard I/O operations

#define MAIN_QUEUE_SIZE (8)             // Define the size of the message queue
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];  // Create the message queue

#define INTERVAL (1U * US_PER_SEC)      // Define the interval time in microseconds

static xtimer_ticks32_t last_wakeup;   // Variable to store the last wake-up time

// Function to simulate sending a message using the message queue
typedef struct {
    msg_t *queue;
    size_t size;
} mbox_t;

void mbox_init(msg_t *queue, size_t size) {
    (void)size; // Placeholder for actual mailbox init if further implementation needed
    msg_init_queue(queue, MAIN_QUEUE_SIZE);
}

void mbox_post(mbox_t *mbox, msg_t *msg) {
    msg_send(msg, thread_getpid());
}

int main(void) {
    puts("RIOT Message Box Example Started\n");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_t mbox;
    mbox_init(_main_msg_queue, MAIN_QUEUE_SIZE);

    last_wakeup = xtimer_now();

    while (1) {
        // Wait for the interval to pass
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);

        // Create a new message
        msg_t msg;
        msg.type = 0x1234; // Example message type

        // Post the message to the queue
        mbox_post(&mbox, &msg);

        puts("Message sent!\n");
    }
    return 0;
}