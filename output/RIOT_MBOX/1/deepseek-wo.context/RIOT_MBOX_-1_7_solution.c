
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "ztimer.h"
#include "mbox.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[STACKSIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        if (_mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received message: %d\n", (int)msg.content.value);
        }
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, 8);

    thread_create(thread_stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST,
                  event_thread, NULL, "event_thread");

    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        printf("Sent message: %d\n", (int)msg.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
