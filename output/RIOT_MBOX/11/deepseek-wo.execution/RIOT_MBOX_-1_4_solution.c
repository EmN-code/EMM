
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

// Define the message structure
typedef struct {
    int type;
    void *content;
} msg_t;

// Define the mailbox structure
typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    atomic_int read_pos;
    atomic_int write_pos;
} mbox_t;

// Initialize the mailbox
static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    atomic_store(&mbox->read_pos, 0);
    atomic_store(&mbox->write_pos, 0);
}

// Put a message into the mailbox
static inline int mbox_put(mbox_t *mbox, msg_t *msg) {
    int next_write_pos = (atomic_load(&mbox->write_pos) + 1) % mbox->queue_size;
    if (next_write_pos == atomic_load(&mbox->read_pos)) {
        return 0; // Queue is full
    }
    mbox->queue[atomic_load(&mbox->write_pos)] = *msg;
    atomic_store(&mbox->write_pos, next_write_pos);
    return 1;
}

// Get a message from the mailbox
static inline int mbox_get(mbox_t *mbox, msg_t *msg) {
    if (atomic_load(&mbox->read_pos) == atomic_load(&mbox->write_pos)) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[atomic_load(&mbox->read_pos)];
    atomic_store(&mbox->read_pos, (atomic_load(&mbox->read_pos) + 1) % mbox->queue_size);
    return 1;
}

// Function to send messages at intervals
void sender_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    int count = 0;

    while (true) {
        msg.type = count++;
        msg.content = NULL;
        mbox_put(mbox, &msg);
        sleep(1); // Send a message every second
    }
}

// Function to receive messages
void receiver_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (true) {
        if (mbox_get(mbox, &msg)) {
            printf("Received message with type: %d\n", msg.type);
        }
    }
}

int main() {
    msg_t msg_queue[16]; // Queue size must be a power of two
    mbox_t mbox;

    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Create sender thread
    pthread_t sender;
    pthread_create(&sender, NULL, (void *(*)(void *))sender_thread, &mbox);

    // Create receiver thread
    pthread_t receiver;
    pthread_create(&receiver, NULL, (void *(*)(void *))receiver_thread, &mbox);

    // Wait for threads to finish (they won't in this example)
    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}
