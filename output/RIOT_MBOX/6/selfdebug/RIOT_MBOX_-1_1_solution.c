#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define MSG_TYPE               (0x1234)
#define MSG_QUEUE_SIZE         (8)

// Message type
typedef struct {
    int data;
} message_t;

// Define a message queue
static msg_t msg_queue[MSG_QUEUE_SIZE];

// Thread stacks
static char sender_thread_stack[THREAD_STACKSIZE_MAIN];
static char receiver_thread_stack[THREAD_STACKSIZE_MAIN];

// Receiver thread function
void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    message_t *received_message;
    while (1) {
        // Block until a message is received
        msg_receive(&msg);
        received_message = (message_t *)msg.content.ptr;
        printf("Received message with data: %d\n", received_message->data);
    }
    return NULL;
}

// Sender thread function
void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    message_t message;
    while (1) {
        // Prepare the message
        message.data = 42;  // Example data
        msg.content.ptr = &message;

        // Send the message to the receiver thread
        msg_send(&msg, thread_get_unchecked(receiver_pid));
    
        // Sleep for 1 second
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create sender and receiver threads
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                              THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                              receiver, NULL, "receiver");

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sender, NULL, "sender");

    return 0;
}