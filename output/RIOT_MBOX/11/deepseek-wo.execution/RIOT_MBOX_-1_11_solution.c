
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

typedef struct {
    int value;
} msg_t;

typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    unsigned int read_pos;
    unsigned int write_pos;
} mbox_t;

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->read_pos = 0;
    mbox->write_pos = 0;
}

static inline int mbox_put(mbox_t *mbox, msg_t *msg) {
    unsigned int next_write_pos = (mbox->write_pos + 1) & (mbox->queue_size - 1);
    if (next_write_pos == mbox->read_pos) {
        return 0; // Queue is full
    }
    mbox->queue[mbox->write_pos] = *msg;
    mbox->write_pos = next_write_pos;
    return 1;
}

static inline int mbox_get(mbox_t *mbox, msg_t *msg, int blocking) {
    if (mbox->read_pos == mbox->write_pos) {
        if (!blocking) {
            return 0; // Queue is empty and non-blocking
        }
        // Blocking implementation would go here
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) & (mbox->queue_size - 1);
    return 1;
}

void sender_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    int count = 0;

    while (1) {
        msg.value = count++;
        if (mbox_put(mbox, &msg)) {
            printf("Sent message: %d\n", msg.value);
        } else {
            printf("Failed to send message: %d\n", msg.value);
        }
        sleep(1);
    }
}

void receiver_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        if (mbox_get(mbox, &msg, 1)) {
            printf("Received message: %d\n", msg.value);
        } else {
            printf("Failed to receive message\n");
        }
    }
}

int main() {
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;

    mbox_init(&mbox, queue, QUEUE_SIZE);

    if (pthread_create(&sender_thread, NULL, (void *(*)(void *))sender_thread, &mbox) != 0) {
        perror("Failed to create sender thread");
        return 1;
    }

    if (pthread_create(&receiver_thread, NULL, (void *(*)(void *))receiver_thread, &mbox) != 0) {
        perror("Failed to create receiver thread");
        return 1;
    }

    pthread_join(sender_thread, NULL);
    pthread_join(receiver_thread, NULL);

    return 0;
}
