#include <ztimer.h>  // Header for timers
#include <stdio.h>   // Header for input and output
#include <thread.h>  // Header for threading
#include <msg.h>     // Header for message utility functions
#include <stdlib.h>  // Header for standard functions
#include <stdbool.h> // Header for boolean types

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)

static char sender_stack[THREAD_STACKSIZE_MAIN];  // Stack for sender thread
static mbox_t mbox;  // Define a mailbox
static msg_t mbox_buffer[8];  // Buffer for messages, must be a power of two

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value++;  // Increment message content
        mbox_put(&mbox, &msg);  // Put message in mailbox
        printf("Message sent: %" PRIu32 "\n", msg.content.value);
        ztimer_sleep(ZTIMER_SEC, 1);  // Sleep for 1 second
    }

    return NULL;
}

int main(void) {
    msg_init_queue(mbox_buffer, 8);  // Initialize message queue
    mbox_init(&mbox, mbox_buffer, 8);  // Initialize mailbox

    if (thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                      sender, NULL, "sender") <= KERNEL_PID_UNDEF) {
        puts("Error creating thread");
        return 1;
    }

    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg) == 1) {
            printf("Message received: %" PRIu32 "\n", msg.content.value);
        }
    }

    return 0;
}