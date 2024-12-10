
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>

#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#define QUEUE_SIZE 8

typedef struct {
    msg_t queue[QUEUE_SIZE];
    unsigned int queue_size;
} mbox_t;

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size)
{
    mbox_t m = { .queue = queue, .queue_size = queue_size };
    *mbox = m;
}

int _mbox_get(mbox_t *mbox, msg_t *msg, int blocking)
{
    (void)mbox;
    (void)msg;
    (void)blocking;
    return 0;
}

int main(void)
{
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;

    mbox_init(&mbox, queue, QUEUE_SIZE);

    while (1) {
        msg_t msg;
        int result = _mbox_get(&mbox, &msg, 1);
        if (result) {
            printf("Message received\n");
        } else {
            printf("No message received\n");
        }
        sleep(1);
    }

    return 0;
}
