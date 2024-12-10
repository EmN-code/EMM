
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "event.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];
static mbox_t mbox;

static void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, 8);

    // Initialize the message box
    mbox_init(&mbox, msg_queue, 8);

    // Create the event thread
    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    // Send events at intervals
    xtimer_t timer;
    msg_t event_msg;
    uint32_t interval = 1000000; // 1 second

    while (1) {
        event_msg.content.value = 1; // Example event value
        mbox_put(&mbox, &event_msg);
        xtimer_usleep(interval);
    }

    return 0;
}
