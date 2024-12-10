#include <thread.h>
#include <msg.h>
#include <kernel.h>
#include <stdio.h>

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t m;
    while (1) {
        // Receive a message and print its value
        msg_receive(&m);
        printf("Received message with value: %u\n", (unsigned)m.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t m;
    m.type = 0;  // Define a message type
    while (1) {
        // Set the message content
        m.content.value = 42;
        // Send the message to the receiver thread
        msg_send(&m, receiver_pid);
        // Sleep for a while before sending the next message
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue for the receiver thread
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE,
                                 RECEIVER_PRIORITY, THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    thread_create(sender_stack, SENDER_STACKSIZE,
                  SENDER_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    // No need to join the threads as this is a continuous program
    return 0;
}