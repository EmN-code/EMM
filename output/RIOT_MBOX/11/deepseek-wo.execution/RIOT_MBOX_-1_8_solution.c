
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/udp.h>

#define QUEUE_SIZE 8

typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    atomic_int read_pos;
    atomic_int write_pos;
} mbox_t;

void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    atomic_store(&mbox->read_pos, 0);
    atomic_store(&mbox->write_pos, 0);
}

int mbox_get(mbox_t *mbox, msg_t *msg, int blocking) {
    while (1) {
        int rpos = atomic_load(&mbox->read_pos);
        int wpos = atomic_load(&mbox->write_pos);

        if (rpos != wpos) {
            *msg = mbox->queue[rpos];
            atomic_store(&mbox->read_pos, (rpos + 1) % mbox->queue_size);
            return 1;
        }

        if (!blocking) {
            return 0;
        }

        usleep(100); // Sleep for a short time to avoid busy-waiting
    }
}

int mbox_put(mbox_t *mbox, msg_t *msg) {
    int wpos = atomic_load(&mbox->write_pos);
    int next_wpos = (wpos + 1) % mbox->queue_size;

    if (next_wpos != atomic_load(&mbox->read_pos)) {
        mbox->queue[wpos] = *msg;
        atomic_store(&mbox->write_pos, next_wpos);
        return 1;
    }

    return 0;
}

int main() {
    msg_t msg_queue[QUEUE_SIZE];
    mbox_t mbox;
    msg_t msg;

    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    for (unsigned int i = 0; i < 10; i++) {
        msg.content.value = i;
        if (mbox_put(&mbox, &msg)) {
            printf("Sent event %u\n", i);
        } else {
            printf("Failed to send event %u\n", i);
        }
        usleep(1000000); // Sleep for 1 second
    }

    for (unsigned int i = 0; i < 10; i++) {
        if (mbox_get(&mbox, &msg, 1)) {
            printf("Received event %u\n", msg.content.value);
        } else {
            printf("Failed to receive event\n");
        }
    }

    return 0;
}
