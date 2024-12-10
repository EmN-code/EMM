
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time_units.h>
#include <kernel_defines.h>

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
    // Placeholder implementation for _mbox_get
    return 0;
}

void sender_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        // Simulate sending an event
        msg.content.value = rand();
        _mbox_get(mbox, &msg, 0); // Non-blocking get
        sleep(1); // Send event every 1 second
    }
}

int main(void)
{
    mbox_t mbox;
    msg_t queue[QUEUE_SIZE];

    mbox_init(&mbox, queue, QUEUE_SIZE);

    // Create sender thread
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  (thread_task_func_t)sender_thread, &mbox, "sender");

    // Main thread can do other work or sleep
    while (1) {
        sleep(10);
    }

    return 0;
}
