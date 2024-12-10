
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

typedef struct {
    void *next;
    void *data;
} msg_t;

typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    unsigned int read_pos;
    unsigned int write_pos;
} mbox_t;

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size)
{
    mbox_t m = {
        .queue = queue,
        .queue_size = queue_size,
        .read_pos = 0,
        .write_pos = 0
    };
    *mbox = m;
}

static inline int mbox_put(mbox_t *mbox, msg_t *msg)
{
    unsigned int next_write_pos = (mbox->write_pos + 1) & (mbox->queue_size - 1);
    if (next_write_pos == mbox->read_pos) {
        return 0; // Queue is full
    }
    mbox->queue[mbox->write_pos] = *msg;
    mbox->write_pos = next_write_pos;
    return 1;
}

static inline int mbox_get(mbox_t *mbox, msg_t *msg)
{
    if (mbox->read_pos == mbox->write_pos) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) & (mbox->queue_size - 1);
    return 1;
}

int main()
{
    msg_t msg_queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    msg_t msg = { .data = (void *)1234 };
    mbox_put(&mbox, &msg);

    msg_t received_msg;
    if (mbox_get(&mbox, &received_msg)) {
        printf("Received message with data: %ld\n", (uintptr_t)received_msg.data);
    } else {
        printf("No message received\n");
    }

    return 0;
}
