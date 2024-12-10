
#include <stdio.h>
#include <thread.h>
#include <mbox.h>
#include <xtimer.h>
#include <msg.h>

#define QUEUE_SIZE 8
#define PRIORITY   THREAD_PRIORITY_MAIN - 1
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);  // Send a message every second
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    thread_create(NULL, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
