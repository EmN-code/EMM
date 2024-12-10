#include "msg.h"
#include "xtimer.h"
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC) // Interval of 1 second
#define MBOX_SIZE 8  // Must be power of two

static msg_t queue[MBOX_SIZE];
static mbox_t mbox;

static void *event_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234; // Example message type

    while (1) {
        xtimer_sleep(1);  // Wait for 1 second
        if (mbox_try_put(&mbox, &msg) != 1) {
            printf("Failed to put message into the mailbox\n");
        } else {
            printf("Message sent!\n");
        }
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, queue, MBOX_SIZE);

    kernel_pid_t event_pid = thread_create(
        (char[THREAD_STACKSIZE_MAIN]){0}, THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN - 1, 0, event_thread, NULL, "event_thread");

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg) == 1) {
            printf("Received message with type: %d\n", msg.type);
        }
    }

    return 0;
}