
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include <stdio.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        xtimer_sleep(1);  // Send event every 1 second
        msg.content.value++;
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;

    // Initialize the message box
    mbox_init(&mbox, msg_queue, 8);

    // Create the event sender thread
    thread_create(stack, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    while (1) {
        // Get message from the message box
        mbox_get(&mbox, &msg, 0);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
