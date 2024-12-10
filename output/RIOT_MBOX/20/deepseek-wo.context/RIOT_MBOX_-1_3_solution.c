
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

void *event_thread(void *arg)
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
    msg_t msg;

    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Create the event thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                               event_thread, NULL, "event_thread");

    // Send events at intervals
    while (1) {
        msg.content.value = 1; // Example event value
        mbox_put(&mbox, &msg);
        xtimer_sleep(2); // Send event every 2 seconds
    }

    return 0;
}
