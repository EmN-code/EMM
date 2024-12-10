
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1234; // Example value
        mbox_put(&mbox, &msg);
        xtimer_sleep(5); // Send event every 5 seconds
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, 8);
    mbox_init(&mbox, msg_queue, 8);

    thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
