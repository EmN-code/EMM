
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "msg.h"
#include <stdio.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static char stack[STACKSIZE];

void *event_handler(void *arg)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event: %" PRIu32 "\n", (uint32_t)msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_t msg;
    thread_t *handler_thread;

    mbox_init(&mbox, NULL);

    handler_thread = thread_create(stack, sizeof(stack), PRIORITY,
                                   THREAD_CREATE_STACKTEST, event_handler, NULL, "event_handler");

    while (1) {
        xtimer_sleep(5);
        msg.content.value = (uint32_t)xtimer_now_usec();
        mbox_put(&mbox, &msg);
    }

    return 0;
}
