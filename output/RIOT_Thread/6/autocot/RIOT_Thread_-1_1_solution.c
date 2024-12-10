#include <msg.h> // RIOT's messaging interface
#include <thread.h> // Threading support in RIOT
#include <stdint.h> // Provides standard integer types

// Thread stack size and priorities
#define THREAD_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_THREAD_SENDER (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD_RECEIVER (THREAD_PRIORITY_MAIN - 2)

// Thread variables
static char stack_sender[THREAD_STACK_SIZE];
static char stack_receiver[THREAD_STACK_SIZE];

// Message queue
msg_t msg_queue_sender[8];
msg_t msg_queue_receiver[8];

// Thread function declarations
void *thread_sender_func(void *arg);
void *thread_receiver_func(void *arg);

int main(void) {
    // Initialize the receiver thread's message queue
    msg_init_queue(msg_queue_receiver, 8);

    // Create the sender thread
    thread_create(stack_sender, THREAD_STACK_SIZE, PRIORITY_THREAD_SENDER, THREAD_CREATE_STACKTEST,
                  thread_sender_func, NULL, "sender");

    // Create the receiver thread
    thread_create(stack_receiver, THREAD_STACK_SIZE, PRIORITY_THREAD_RECEIVER, THREAD_CREATE_STACKTEST,
                  thread_receiver_func, NULL, "receiver");

    // Main thread can perform other initialization tasks if necessary
    //...

    return 0;
}

void *thread_sender_func(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;
    msg.content.value = 42; // Arbitrary message content

    while (1) {
        // Send a message to the receiver thread
        msg_send(&msg, thread_getpid_of(PID(receiver)));

        // Sleep or perform other tasks as needed
        thread_yield();
    }

    return NULL;
}

void *thread_receiver_func(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;

    while (1) {
        // Receive a message (blocking)
        msg_receive(&msg);

        // Process the message
        printf("Received message with value: %d\n", msg.content.value);

        // Reply to the sender, if necessary
        // msg_reply(&msg, &reply_msg);

        // Perform any additional processing
    }

    return NULL;
}