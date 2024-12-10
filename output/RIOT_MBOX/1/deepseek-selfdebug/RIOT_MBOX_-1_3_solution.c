
#include <stdio.h>
#include <thread.h>
#include <mbox.h>
#include <xtimer.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static char stack[STACKSIZE];

void *event_handler(void *arg)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received event: %d\n", (int)msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_t msg;
    thread_t *handler_thread;

    mbox_init(&mbox);

    handler_thread = thread_create(stack, sizeof(stack), PRIORITY,
                                   THREAD_CREATE_STACKTEST, event_handler, NULL, "event_handler");

    while (1) {
        msg.content.value = 1;
        mbox_put(&mbox, &msg);
        xtimer_sleep(5);
    }

    return 0;
}
