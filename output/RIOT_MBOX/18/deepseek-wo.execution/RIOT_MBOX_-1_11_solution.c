
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MSG_QUEUE_SIZE 8

typedef struct {
    int sender_pid;
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
    unsigned int next_write_pos = (mbox->write_pos + 1) % mbox->queue_size;
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
        // Blocking case: busy-wait until a message is available
        while (mbox->read_pos == mbox->write_pos);
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

void sender_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(mbox, &msg);
        sleep(1); // Send event every second
    }
}

int main() {
    msg_t msg_queue[MSG_QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    pthread_t sender_tid;
    pthread_create(&sender_tid, NULL, (void *(*)(void *))sender_thread, &mbox);

    msg_t received_msg;
    while (1) {
        if (mbox_get(&mbox, &received_msg, 1)) {
            printf("Received message with value: %u\n", received_msg.content.value);
        }
    }

    return 0;
}
