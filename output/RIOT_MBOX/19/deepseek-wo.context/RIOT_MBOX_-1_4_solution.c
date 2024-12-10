
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;
static mbox_t mbox;
static msg_t msg_queue[8];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);  // Send an event every second
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
        printf("Received event with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, 8);

    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                               event_sender, NULL, "event_sender");

    thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  event_receiver, NULL, "event_receiver");

    return 0;
}
