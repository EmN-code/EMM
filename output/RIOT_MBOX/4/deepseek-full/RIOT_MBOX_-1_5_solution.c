
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define QUEUE_SIZE 8
#define INTERVAL_SEC 5

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

void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->read_pos = 0;
    mbox->write_pos = 0;
}

int mbox_put(mbox_t *mbox, msg_t *msg) {
    unsigned int next_write_pos = (mbox->write_pos + 1) % mbox->queue_size;
    if (next_write_pos == mbox->read_pos) {
        return 0; // Queue is full
    }
    mbox->queue[mbox->write_pos] = *msg;
    mbox->write_pos = next_write_pos;
    return 1;
}

int mbox_get(mbox_t *mbox, msg_t *msg) {
    if (mbox->read_pos == mbox->write_pos) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

void send_event(mbox_t *mbox) {
    msg_t msg;
    msg.data = (void *)"Event Data";
    mbox_put(mbox, &msg);
}

int main() {
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, queue, QUEUE_SIZE);

    while (1) {
        send_event(&mbox);
        printf("Event sent at %ld\n", time(NULL));
        sleep(INTERVAL_SEC);
    }

    return 0;
}
