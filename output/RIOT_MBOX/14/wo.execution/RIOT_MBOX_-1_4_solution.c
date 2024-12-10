#include <stdio.h>               // To declare printf
#include <stdlib.h>              // Standard library for general purposes
#include <thread.h>              // For mbox_recipient_pid
#include <xtimer.h>              // For US_PER_SEC and xtimer

#define MBOX_SIZE           (8U)
#define INTERVAL_US         (1000000U) // 1 second in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[MBOX_SIZE];

// Thread function to receive messages
void *recipient(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        // Receive message from message queue
        msg_receive(&msg);
        printf("Received message ID: %u\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Create the recipient thread
    kernel_pid_t recipient_pid = thread_create(stack, sizeof(stack),
                                               THREAD_PRIORITY_MAIN - 1,
                                               THREAD_CREATE_STACKTEST,
                                               recipient, NULL, "recipient");

    // Initialize message queue
    mbox_recipient_pid(&msg_queue, MBOX_SIZE);

    printf("Starting message emitters and message box setup...\n");

    // Periodically send messages
    msg_t msg;
    unsigned int counter = 0;
    while (1) {
        msg.content.value = counter++;
        msg_send(&msg, recipient_pid);
        xtimer_usleep(INTERVAL_US);  // Sleep for 1 second
    }

    return 0;
}