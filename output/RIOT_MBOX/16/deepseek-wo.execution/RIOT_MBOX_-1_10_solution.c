
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <esp_system.h>

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

int _mbox_get(mbox_t *mbox, msg_t *msg, int blocking)
{
    (void)blocking; // Ignoring blocking for simplicity
    if (mbox->read_pos == mbox->write_pos) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

int main()
{
    msg_t msg_queue[8];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, 8);

    msg_t msg;
    msg.content.value = 42;

    // Simulate sending events at intervals
    for (int i = 0; i < 5; i++) {
        mbox.queue[mbox.write_pos] = msg;
        mbox.write_pos = (mbox.write_pos + 1) % mbox.queue_size;
        sleep(1); // Simulate interval
    }

    // Simulate receiving messages
    for (int i = 0; i < 5; i++) {
        if (_mbox_get(&mbox, &msg, 0)) {
            printf("Received message with value: %d\n", msg.content.value);
        } else {
            printf("No message available\n");
        }
    }

    return 0;
}
