#include <xtimer.h>
#include <msg.h>
#include <kernel.h>
#include <stdio.h>

// Define the size of the message queue
#define MESSAGE_QUEUE_SIZE (4)

// Define the time interval for sending messages in microseconds
#define MESSAGE_INTERVAL (1000000) // 1 second

// Define the msg_t type for the message box
msg_t message_queue[MESSAGE_QUEUE_SIZE];

// Define the mailbox structure
mbox_t mbox;

// Function that sends messages at intervals
void sender(void) {
    int counter = 0;
    msg_t message;
    while (1) {
        message.content.value = counter++;
        mbox_put(&mbox, &message);
        printf("Message sent: %d\n", message.content.value);
        xtimer_usleep(MESSAGE_INTERVAL);
    }
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, message_queue, MESSAGE_QUEUE_SIZE);

    // Initialize a thread for sending messages
    char thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sender, NULL, "sender_thread");

    while (1) {
        // Main thread can perform other tasks or simply idle
        xtimer_sleep(1);
    }
    return 0;
}