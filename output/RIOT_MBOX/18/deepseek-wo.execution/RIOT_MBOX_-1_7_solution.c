
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

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

static inline int mbox_get(mbox_t *mbox, msg_t *msg) {
    if (mbox->read_pos == mbox->write_pos) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) & (mbox->queue_size - 1);
    return 1;
}

int main() {
    msg_t msg_queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    // Send events at intervals
    while (1) {
        if (mbox_put(&mbox, &msg)) {
            printf("Message sent: %u\n", (unsigned int)msg.content.value);
        } else {
            printf("Message queue is full!\n");
        }
        sleep(1); // Sleep for 1 second
    }

    return 0;
}
