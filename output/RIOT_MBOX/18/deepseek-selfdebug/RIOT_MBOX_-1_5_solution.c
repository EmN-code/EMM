
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_queue[8];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(2);
        msg_send(&msg, thread_getpid());
    }
}

int main(void)
{
    msg_t msg;

    // Initialize the message box
    mbox_init(&mbox, msg_queue, 8);

    // Create the sender thread
    thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    while (1) {
        // Get a message from the message box
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
