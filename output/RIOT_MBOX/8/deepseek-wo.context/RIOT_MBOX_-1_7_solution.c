
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include <stdio.h>

#define QUEUE_SIZE (16)
#define INTERVAL (1000000U)  // 1 second in microseconds

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL);
    }
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Create a sender thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    // Main thread acts as the receiver
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}
