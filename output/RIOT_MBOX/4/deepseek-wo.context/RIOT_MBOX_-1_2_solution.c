
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_queue[8];

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        msg.content.value = 42; // Example value
        mbox_put(&mbox, &msg);
        xtimer_sleep(1); // Send a message every second
    }
}

void receiver_thread(void)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Create sender thread
    thread_create(
        NULL,
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        (thread_task_func_t)sender_thread,
        NULL,
        "sender"
    );

    // Create receiver thread
    thread_create(
        NULL,
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        (thread_task_func_t)receiver_thread,
        NULL,
        "receiver"
    );

    return 0;
}
