#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include <stdio.h>

#define QUEUE_SIZE (8)
#define INTERVAL (3000U)

static mbox_t mbox;
static msg_t queue[QUEUE_SIZE];

void sender(void) {
    msg_t msg;
    while (1) {
        msg.type = 0x1234; // message type
        if (mbox_try_send(&mbox, &msg) == 1) {
            puts("Message sent.");
        }
        else {
            puts("Failed to send message.");
        }
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
}

void receiver(void) {
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        puts("Message received.");
    }
}

int main(void) {
    mbox_init(&mbox, queue, QUEUE_SIZE);
    puts("Initialized mailbox and queue.");

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 2, 0, receiver, NULL, "receiver");

    while (1) {}
    return 0;
}
