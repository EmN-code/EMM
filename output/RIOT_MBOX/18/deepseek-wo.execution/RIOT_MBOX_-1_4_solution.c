
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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
} mbox_t;

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox_t m = {queue, queue_size};
    *mbox = m;
}

int main(void) {
    msg_t msg_queue[QUEUE_SIZE];
    mbox_t mbox;

    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    while (1) {
        // Simulate sending events at intervals
        sleep(1);
        printf("Event sent\n");
    }

    return 0;
}
