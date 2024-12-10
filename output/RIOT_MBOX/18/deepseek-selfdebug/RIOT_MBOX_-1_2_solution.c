
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "msg.h"

#define QUEUE_SIZE (8)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example event value

    while (1) {
        xtimer_sleep(1); // Send event every 1 second
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");
    thread_create(NULL, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_receiver, NULL, "event_receiver");

    return 0;
}
