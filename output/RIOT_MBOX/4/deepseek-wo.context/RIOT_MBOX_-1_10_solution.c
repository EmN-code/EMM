
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_array[8];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        xtimer_sleep(5);  // Send an event every 5 seconds
        msg.content.value = 1;  // Dummy event value
        mbox_put(&mbox, &msg);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_array, 8);
    mbox_init(&mbox, msg_array, 8);

    char stack[THREAD_STACKSIZE];
    thread_create(stack, THREAD_STACKSIZE, THREAD_PRIORITY, THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);  // Blocking call to get message from mbox
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
