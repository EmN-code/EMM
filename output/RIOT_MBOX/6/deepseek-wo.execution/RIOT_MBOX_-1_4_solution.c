
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_SIZE 8

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
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
    unsigned int next_write_pos = (mbox->write_pos + 1) % mbox->queue_size;
    if (next_write_pos == mbox->read_pos) {
        return 0; // Queue is full
    }
    mbox->queue[mbox->write_pos] = *msg;
    mbox->write_pos = next_write_pos;
    return 1;
}

static inline int mbox_get(mbox_t *mbox, msg_t *msg, int blocking)
{
    if (mbox->read_pos == mbox->write_pos) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

int main(void)
{
    msg_t msg_queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    msg_t msg = { .type = 1, .content.value = 42 };
    mbox_put(&mbox, &msg);

    msg_t received_msg;
    if (mbox_get(&mbox, &received_msg, 1)) {
        printf("Received message with type %u and value %u\n", received_msg.type, received_msg.content.value);
    } else {
        printf("No message received\n");
    }

    return 0;
}
