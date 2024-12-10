
#include "kernel_defines.h"
#include "mbox.h"
#include "thread.h"
#include "xtimer.h"
#include "stdio.h"
#include "unistd.h"

#define QUEUE_SIZE 8
#define PRIORITY  1
#define STACKSIZE 512

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        _mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        _mbox_get(&mbox, &msg, 1);
        printf("Received message: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, STACKSIZE, PRIORITY, 0, sender_thread, NULL, "sender");
    thread_create(NULL, STACKSIZE, PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}
