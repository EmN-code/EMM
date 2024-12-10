
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];
static mbox_t mbox;

void *event_sender(void *arg)
{
    (void)arg;
    kernel_pid_t sender_pid = thread_getpid();

    while (1) {
        msg_t msg;
        msg.content.value = 42; // Example event value
        mbox_put(&mbox, &msg);
        xtimer_sleep(1); // Send event every 1 second
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, 8);

    thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
