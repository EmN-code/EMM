
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

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

void sender_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    uint32_t event_count = 0;

    while (1) {
        msg.content.value = event_count++;
        if (mbox_put(mbox, &msg)) {
            printf("Event %lu sent\n", (unsigned long)msg.content.value);
        } else {
            printf("Failed to send event %lu\n", (unsigned long)msg.content.value);
        }
        sleep(1);
    }
}

void receiver_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        if (mbox_get(mbox, &msg)) {
            printf("Event %lu received\n", (unsigned long)msg.content.value);
        } else {
            printf("No event received\n");
        }
        sleep(1);
    }
}

int main()
{
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;

    mbox_init(&mbox, queue, QUEUE_SIZE);

    thread_create(NULL, 1024, 1, 0, (thread_task_func_t)sender_thread, &mbox, "sender");
    thread_create(NULL, 1024, 1, 0, (thread_task_func_t)receiver_thread, &mbox, "receiver");

    return 0;
}
