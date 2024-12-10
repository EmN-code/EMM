#include <thread.h>
#include <msg.h>
#include <stdio.h>
#include <xtimer.h>

// Define stack size and priority levels for threads
#define THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_SENDER     (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_RECEIVER   (THREAD_PRIORITY_MAIN - 2)

// Define the message queue size
#define MSG_QUEUE_SIZE      (8)

// Message queue for the receiver thread
static msg_t msg_queue[MSG_QUEUE_SIZE];

// Thread stack memory allocation
static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

// Function for the receiver thread
void *receiver(void *arg) {
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        // Block and wait for a message
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

// Function for the sender thread
void *sender(void *arg) {
    msg_t msg;
    msg.type = 0x1234; // Example message type

    while (1) {
        msg.content.value = (uint32_t)arg;
        // Send the message to the receiver
        msg_send(&msg, thread_getpid_of(receiver_stack));
        printf("Sender: Sent message with value %lu\n", (unsigned long)msg.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    // Set up receiver thread
    if (thread_create(receiver_stack, sizeof(receiver_stack), PRIORITY_RECEIVER,
                      THREAD_CREATE_STACKTEST, receiver, NULL, "receiver") <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create receiver thread\n");
        return 1;
    }

    // Set up sender thread
    if (thread_create(sender_stack, sizeof(sender_stack), PRIORITY_SENDER,
                      THREAD_CREATE_STACKTEST, sender, (void *)42, "sender") <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create sender thread\n");
        return 1;
    }

    return 0;
}