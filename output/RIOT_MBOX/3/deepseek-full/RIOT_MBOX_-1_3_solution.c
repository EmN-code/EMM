
#include <stdio.h>
#include <unistd.h> // for sleep function

// Define a simple message structure
typedef struct {
    int event;
} msg_t;

// Define a simple mailbox structure
typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    unsigned int head;
    unsigned int tail;
} mbox_t;

// Initialize the mailbox
void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->head = 0;
    mbox->tail = 0;
}

// Add a message to the mailbox
int mbox_put(mbox_t *mbox, msg_t *msg) {
    if (((mbox->tail + 1) % mbox->queue_size) == mbox->head) {
        return 0; // Queue is full
    }
    mbox->queue[mbox->tail] = *msg;
    mbox->tail = (mbox->tail + 1) % mbox->queue_size;
    return 1;
}

// Get a message from the mailbox
int mbox_get(mbox_t *mbox, msg_t *msg) {
    if (mbox->head == mbox->tail) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[mbox->head];
    mbox->head = (mbox->head + 1) % mbox->queue_size;
    return 1;
}

int main() {
    // Define a queue for the mailbox
    msg_t queue[16];
    mbox_t mbox;

    // Initialize the mailbox
    mbox_init(&mbox, queue, sizeof(queue) / sizeof(queue[0]));

    // Simulate sending events at intervals
    for (int i = 0; i < 10; i++) {
        msg_t msg = { .event = i };
        if (mbox_put(&mbox, &msg)) {
            printf("Event %d sent\n", i);
        } else {
            printf("Mailbox full, event %d not sent\n", i);
        }
        sleep(1); // Simulate interval
    }

    // Simulate receiving events
    msg_t received_msg;
    while (mbox_get(&mbox, &received_msg)) {
        printf("Event %d received\n", received_msg.event);
    }

    return 0;
}
