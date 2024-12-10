
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <time_units.h>
#include <stdlib.h>
#include <mutex.h>
#include <sys/socket.h>
#include <kernel_defines.h>

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
    mutex_t mutex;
} mbox_t;

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size)
{
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->read_pos = 0;
    mbox->write_pos = 0;
    mutex_init(&mbox->mutex);
}

static inline int mbox_put(mbox_t *mbox, msg_t *msg)
{
    mutex_lock(&mbox->mutex);
    unsigned int next_write_pos = (mbox->write_pos + 1) % mbox->queue_size;
    if (next_write_pos == mbox->read_pos) {
        mutex_unlock(&mbox->mutex);
        return 0;
    }
    mbox->queue[mbox->write_pos] = *msg;
    mbox->write_pos = next_write_pos;
    mutex_unlock(&mbox->mutex);
    return 1;
}

static inline int mbox_get(mbox_t *mbox, msg_t *msg, int blocking)
{
    while (1) {
        mutex_lock(&mbox->mutex);
        if (mbox->read_pos != mbox->write_pos) {
            *msg = mbox->queue[mbox->read_pos];
            mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
            mutex_unlock(&mbox->mutex);
            return 1;
        }
        mutex_unlock(&mbox->mutex);
        if (!blocking) {
            return 0;
        }
        usleep(1000); // Sleep for a short time to avoid busy-waiting
    }
}

void *sender_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        if (mbox_put(mbox, &msg)) {
            printf("Message sent\n");
        } else {
            printf("Message box full\n");
        }
        sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        if (mbox_get(mbox, &msg, 1)) {
            printf("Message received: type=%u, value=%u\n", msg.type, msg.content.value);
        } else {
            printf("Message box empty\n");
        }
    }

    return NULL;
}

int main(void)
{
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, queue, QUEUE_SIZE);

    pthread_t sender, receiver;
    pthread_create(&sender, NULL, sender_thread, &mbox);
    pthread_create(&receiver, NULL, receiver_thread, &mbox);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}
