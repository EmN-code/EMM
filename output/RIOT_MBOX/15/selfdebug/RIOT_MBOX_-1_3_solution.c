#include <xtimer.h>  // For using xtimer_set for time-based events.
#include <stdlib.h>  // Standard library for general utilities.
#include <stdio.h>   // For input/output operations like puts.
#include <kernel_defines.h> // Includes for kernel related macro definitions

#define INTERVAL (1000000U) // 1 second interval for events

// Define a size for the message queue
#define MESSAGE_QUEUE_SIZE (8)

static msg_t message_queue[MESSAGE_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    unsigned int interval = INTERVAL;

    while (1) {
        puts("Sending message at interval...");
        msg.content.value = random_bytes(&msg, sizeof(msg.content.value));
        mbox_try_put(&mbox, &msg);
        xtimer_usleep(interval);
    }
    return NULL;
}

int main(void) {
    mbox_init(&mbox, message_queue, MESSAGE_QUEUE_SIZE);
    puts("Message box initialized. Starting event sender.");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");
    return 0;
}