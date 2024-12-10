
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static char sender_stack[SENDER_THREAD_STACKSIZE];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_sleep(1);  // Send a message every second
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;
    kernel_pid_t sender_pid;

    // Initialize the message queue
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, main_msg_queue);

    // Create the sender thread
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    // Main loop to receive messages from the mailbox
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
