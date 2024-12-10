
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

// Define a simple message structure
typedef struct {
    int type;
    void *content;
} msg_t;

// Define a simple mailbox structure
typedef struct {
    msg_t *queue;
    int head;
    int tail;
    int size;
} mbox_t;

// Function to initialize the mailbox
void mbox_init(mbox_t *mbox, msg_t *queue, int size) {
    mbox->queue = queue;
    mbox->head = 0;
    mbox->tail = 0;
    mbox->size = size;
}

// Function to put a message into the mailbox
int mbox_put(mbox_t *mbox, msg_t *msg) {
    int next = (mbox->tail + 1) % mbox->size;
    if (next != mbox->head) {
        mbox->queue[mbox->tail] = *msg;
        mbox->tail = next;
        return 1;
    }
    return 0;
}

// Function to get a message from the mailbox
int mbox_get(mbox_t *mbox, msg_t *msg) {
    if (mbox->head == mbox->tail) {
        return 0;
    }
    *msg = mbox->queue[mbox->head];
    mbox->head = (mbox->head + 1) % mbox->size;
    return 1;
}

// Function to simulate sending events at intervals
void *event_sender(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    int count = 0;

    while (1) {
        msg.type = count++;
        msg.content = NULL;
        mbox_put(mbox, &msg);
        sleep(1); // Simulate interval
    }

    return NULL;
}

// Main function
int main() {
    mbox_t mbox;
    msg_t queue[10];
    pthread_t sender_thread;

    // Initialize the mailbox
    mbox_init(&mbox, queue, 10);

    // Create a thread to send events
    pthread_create(&sender_thread, NULL, event_sender, &mbox);

    // Main loop to receive messages
    msg_t received_msg;
    while (1) {
        if (mbox_get(&mbox, &received_msg)) {
            printf("Received message with type: %d\n", received_msg.type);
        }
    }

    return 0;
}
