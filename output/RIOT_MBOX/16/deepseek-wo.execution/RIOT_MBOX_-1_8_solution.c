
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>

typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    unsigned int read_pos;
    unsigned int write_pos;
    mutex_t lock;
} mbox_t;

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size)
{
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->read_pos = 0;
    mbox->write_pos = 0;
    mutex_init(&mbox->lock);
}

int _mbox_get(mbox_t *mbox, msg_t *msg, int blocking)
{
    mutex_lock(&mbox->lock);
    if (mbox->read_pos != mbox->write_pos) {
        *msg = mbox->queue[mbox->read_pos];
        mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
        mutex_unlock(&mbox->lock);
        return 1;
    }
    mutex_unlock(&mbox->lock);
    return 0;
}

int main(void)
{
    msg_t queue[8];
    mbox_t mbox;
    msg_t msg;

    mbox_init(&mbox, queue, 8);

    while (1) {
        msg.content.value = rand();
        mbox.queue[mbox.write_pos] = msg;
        mbox.write_pos = (mbox.write_pos + 1) % mbox.queue_size;

        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Received message: %d\n", msg.content.value);
        }

        sleep(1);
    }

    return 0;
}
