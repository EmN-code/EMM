
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <kernel_defines.h>

typedef uint16_t kernel_pid_t;

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
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->read_pos = 0;
    mbox->write_pos = 0;
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

static inline int mbox_get(mbox_t *mbox, msg_t *msg)
{
    if (mbox->read_pos == mbox->write_pos) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

int main()
{
    mbox_t mbox;
    msg_t queue[8];
    msg_t msg;

    mbox_init(&mbox, queue, sizeof(queue) / sizeof(queue[0]));

    msg.type = 1;
    msg.content.value = 42;

    for (int i = 0; i < 10; i++) {
        if (mbox_put(&mbox, &msg)) {
            printf("Message sent: %u\n", (unsigned int)msg.content.value);
        } else {
            printf("Message box full, message not sent.\n");
        }
        sleep(1);
    }

    while (mbox_get(&mbox, &msg)) {
        printf("Message received: %u\n", (unsigned int)msg.content.value);
    }

    return 0;
}
