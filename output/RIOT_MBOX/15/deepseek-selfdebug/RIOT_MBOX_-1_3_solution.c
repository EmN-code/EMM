
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_queue[8];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    uint32_t interval = 1000000; // 1 second in microseconds

    while (1) {
        msg.content.value = 0x1234; // Example message content
        mbox_put(&mbox, &msg);
        xtimer_usleep(interval);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;
    kernel_pid_t sender_pid;

    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Create the event sender thread
    sender_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE),
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        event_sender,
        NULL,
        "event_sender"
    );

    // Main loop to receive messages from the message box
    while (1) {
        mbox_get(&mbox, &msg); // Corrected to use only two arguments
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
