#include <xtimer.h>  // Required for timer functions
#include <thread.h>  // Required for thread functions and operations
#include <msg.h>     // Required for msg_t and message queue operations

#define MAIN_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define QUEUE_SIZE (8)  // Define the size of the message queue

// Declare stacks for threads
char sender_thread_stack[SENDER_THREAD_STACKSIZE];
char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

// Message queue
msg_t msg_queue[QUEUE_SIZE];

// Function prototypes
void *sender_thread(void *arg);
void *receiver_thread(void *arg);

// Sender thread function
void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Payload to send
    while (1) {
        xtimer_sleep(1);  // Wait for a second
        printf("Sender: Sending message...\n");
        msg_send(&msg, thread_getpid());  // Send message to receiver
    }
    return NULL;
}

// Receiver thread function
void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);  // Wait to receive message
        printf("Receiver: Received message with value %d\n", (int)msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Create sender thread with lower priority
    thread_create(sender_thread_stack, SENDER_THREAD_STACKSIZE,
                  THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "Sender Thread");

    // Create receiver thread with higher priority
    thread_create(receiver_thread_stack, RECEIVER_THREAD_STACKSIZE,
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "Receiver Thread");

    // Main thread can do other tasks or idle
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}