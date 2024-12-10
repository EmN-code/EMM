
#include <stdio.h>
#include <unistd.h> // For sleep function

// Define a simple message structure
typedef struct {
    int event_id;
} msg_t;

// Define a simple mailbox structure
typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    unsigned int head;
    unsigned int tail;
} mbox_t;

// Function to initialize the mailbox
void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->head = 0;
    mbox->tail = 0;
}

// Function to put a message into the mailbox
int mbox_put(mbox_t *mbox, msg_t *msg) {
    unsigned int next = (mbox->tail + 1) % mbox->queue_size;
    if (next != mbox->head) { // Check if mailbox is full
        mbox->queue[mbox->tail] = *msg;
        mbox->tail = next;
        return 1; // Success
    }
    return 0; // Failure
}

// Function to get a message from the mailbox
int mbox_get(mbox_t *mbox, msg_t *msg) {
    if (mbox->head != mbox->tail) { // Check if mailbox is empty
        *msg = mbox->queue[mbox->head];
        mbox->head = (mbox->head + 1) % mbox->queue_size;
        return 1; // Success
    }
    return 0; // Failure
}

int main() {
    // Define a mailbox and a message queue
    mbox_t mbox;
    msg_t queue[16];
    msg_t msg;

    // Initialize the mailbox
    mbox_init(&mbox, queue, sizeof(queue) / sizeof(queue[0]));

    // Send events at intervals
    for (int i = 0; i < 10; i++) {
        msg.event_id = i;
        if (mbox_put(&mbox, &msg)) {
            printf("Event %d sent\n", i);
        } else {
            printf("Mailbox full, event %d not sent\n", i);
        }
        sleep(1); // Sleep for 1 second
    }

    // Receive events
    while (mbox_get(&mbox, &msg)) {
        printf("Event %d received\n", msg.event_id);
    }

    return 0;
}
