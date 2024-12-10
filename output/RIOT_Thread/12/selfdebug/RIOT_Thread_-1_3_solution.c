#include <thread.h>  // For thread management
#include <stdio.h>  // For standard input and output

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)

char stack[THREAD_STACKSIZE]; // Thread stack for the sender thread

static void sender_thread(void *arg) {
    (void)arg;  // Suppress unused variable warning
    printf("Sender thread started!\n");

    msg_t msg;
    msg.type = 1;  // Define your message type

    while (1) {
        printf("Sending message\n");
        msg_send(&msg, thread_getpid());  // Send message to the main thread
        xtimer_sleep(2);  // Sleep for 2 seconds
    }
}

int main(void) {
    printf("Main thread started!\n");

    // Structure for message handling
    msg_t msg;

    // Create the sender thread
    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0,
                      sender_thread, NULL, "sender") <= KERNEL_PID_UNDEF) {
        printf("Error creating the thread\n");
        return 1;
    }

    // Wait and process messages
    while (1) {
        // Blocking call to receive the message
        msg_receive(&msg);
        printf("Received message of type: %d\n", (int) msg.type);
    }

    return 0;
}