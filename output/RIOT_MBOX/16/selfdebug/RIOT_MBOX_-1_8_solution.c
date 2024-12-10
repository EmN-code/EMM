#include <stdio.h>     // For printf
#include <thread.h>   // For thread operations like thread_create
#include <msg.h>      // For message queue operations
#include <xtimer.h>   // For timer functionalities
#include <event.h>    // For event functionalities

#define STACKSIZE 1024
#define PRIORITY  (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE 8

// Stack for the sender thread
static char sender_stack[STACKSIZE];

// Define a message type
typedef struct {
    msg_t msg;
} mbox_msg_t;

// Initialize a message box
mbox_t mbox;
msg_t msg_queue[MSG_QUEUE_SIZE];

// Sender function
void *sender(void *arg) {
    (void)arg; // Unused parameter
    mbox_msg_t mbox_msg;
    while (1) {
        // Send a message
        mbox_msg.msg.type = 0x1234; // Arbitrary message type
        mbox_put(&mbox, &mbox_msg.msg, THREAD_PRIORITY_MAIN);
        printf("Sent a message.\n");
        // Wait 1 second
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create sender thread
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), PRIORITY, 0, sender, NULL, "sender");
    if (sender_pid < 0) {
        printf("Failed to create sender thread.\n");
        return 1;
    }
    printf("Sender thread created with PID: %d\n", sender_pid);

    while (1) {
        // Main thread can perform other tasks
        xtimer_sleep(5);
    }
    return 0;
}