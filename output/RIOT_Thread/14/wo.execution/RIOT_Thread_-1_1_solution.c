#include "thread.h"
#include "msg.h"

#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example value to be sent
    while (1) {
        // Send the message to the main thread (receiver)
        msg_send(&msg, thread_getpid());
        // Sleep for a second to throttle sending
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int res;
    while (1) {
        // Receive the message
        res = msg_receive(&msg);
        if (res == 1) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    // Declare thread stacks
    char sender_stack[STACKSIZE];
    char receiver_stack[STACKSIZE];

    // Create Sender and Receiver threads
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack),
                  THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver");
    
    return 0;
}