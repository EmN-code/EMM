
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
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
    atomic_int read_pos;
    atomic_int write_pos;
} mbox_t;

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size)
{
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    atomic_store(&mbox->read_pos, 0);
    atomic_store(&mbox->write_pos, 0);
}

static inline int mbox_put(mbox_t *mbox, msg_t *msg)
{
    int next_write_pos = (atomic_load(&mbox->write_pos) + 1) % mbox->queue_size;
    if (next_write_pos == atomic_load(&mbox->read_pos)) {
        return 0; // Queue is full
    }
    mbox->queue[atomic_load(&mbox->write_pos)] = *msg;
    atomic_store(&mbox->write_pos, next_write_pos);
    return 1;
}

static inline int mbox_get(mbox_t *mbox, msg_t *msg, int blocking)
{
    if (atomic_load(&mbox->read_pos) == atomic_load(&mbox->write_pos)) {
        if (!blocking) {
            return 0; // Queue is empty
        }
        // Blocking implementation would go here
    }
    *msg = mbox->queue[atomic_load(&mbox->read_pos)];
    atomic_store(&mbox->read_pos, (atomic_load(&mbox->read_pos) + 1) % mbox->queue_size);
    return 1;
}

void sender_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    int count = 0;

    while (1) {
        msg.data = (void *)(uintptr_t)count++;
        if (mbox_put(mbox, &msg)) {
            printf("Sent message: %d\n", (int)(uintptr_t)msg.data);
        } else {
            printf("Failed to send message: %d\n", (int)(uintptr_t)msg.data);
        }
        sleep(1);
    }
}

void receiver_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        if (mbox_get(mbox, &msg, 1)) {
            printf("Received message: %d\n", (int)(uintptr_t)msg.data);
        } else {
            printf("Failed to receive message\n");
        }
    }
}

int main()
{
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;

    mbox_init(&mbox, queue, QUEUE_SIZE);

    pthread_t sender, receiver;
    pthread_create(&sender, NULL, (void *(*)(void *))sender_thread, &mbox);
    pthread_create(&receiver, NULL, (void *(*)(void *))receiver_thread, &mbox);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}
