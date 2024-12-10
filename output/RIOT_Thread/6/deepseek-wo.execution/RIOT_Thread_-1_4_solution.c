
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <timex.h>
#include <unistd.h>
#include <stddef.h>

// Define the message structure
typedef struct {
    kernel_pid_t sender_pid;    /**< PID of sending thread. Will be filled in by msg_send. */
    uint16_t type;              /**< Type field. */
    union {
        void *ptr;              /**< Pointer content field. */
        uint32_t value;         /**< Value content field. */
    } content;                  /**< Content of the message. */
} msg_t;

// Define the thread function prototypes
void *sender_thread(void *arg);
void *receiver_thread(void *arg);

// Define the thread stack sizes
#define SENDER_STACKSIZE    (1024)
#define RECEIVER_STACKSIZE  (1024)

// Define the thread priorities
#define SENDER_PRIORITY     (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY   (THREAD_PRIORITY_MAIN - 2)

// Define the message queue size
#define MSG_QUEUE_SIZE      (8)

// Define the message types
#define MSG_TYPE_1          (1)
#define MSG_TYPE_2          (2)

// Main function
int main(void)
{
    // Initialize the message queue
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the sender thread
    kernel_pid_t sender_pid = thread_create(
        malloc(SENDER_STACKSIZE),
        SENDER_STACKSIZE,
        SENDER_PRIORITY,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    // Create the receiver thread
    kernel_pid_t receiver_pid = thread_create(
        malloc(RECEIVER_STACKSIZE),
        RECEIVER_STACKSIZE,
        RECEIVER_PRIORITY,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    // Main thread loop
    while (1) {
        // Main thread can do other tasks here
        sleep(1);
    }

    return 0;
}

// Sender thread function
void *sender_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.type = MSG_TYPE_1;
    msg.content.value = 1234;

    while (1) {
        // Send a message to the receiver thread
        msg_send_receive(&msg, &msg, thread_getpid());

        // Sleep for a while before sending the next message
        sleep(2);
    }

    return NULL;
}

// Receiver thread function
void *receiver_thread(void *arg)
{
    (void)arg;

    msg_t msg;

    while (1) {
        // Receive a message from the sender thread
        msg_receive(&msg);

        // Process the received message
        switch (msg.type) {
            case MSG_TYPE_1:
                printf("Received message of type 1 with value %lu\n", (unsigned long)msg.content.value);
                break;
            case MSG_TYPE_2:
                printf("Received message of type 2 with value %lu\n", (unsigned long)msg.content.value);
                break;
            default:
                printf("Received unknown message type\n");
                break;
        }
    }

    return NULL;
}
