
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_sleep(1);  // Send an event every second
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, 8);

    thread_create(thread_stack, THREAD_STACKSIZE, THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
    }

    return 0;
}
