#include <xtimer.h>
#include <thread.h>
#include <msg.h>

// Define stack size and priorities
#define SENDER_STACK_SIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_STACK_SIZE THREAD_STACKSIZE_DEFAULT
#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)   // Higher priority
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2) // Lower priority

// Declare thread stack areas
char sender_stack[SENDER_STACK_SIZE];
char receiver_stack[RECEIVER_STACK_SIZE];

// Declare a message queue for the receiver
msg_t receiver_queue[8];

// Function for the receiver thread
void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(receiver_queue, 8);

    while (1) {
        // Blocking wait for incoming messages
        msg_receive(&msg);
        printf("Received message: type = %u, value = %u\n", msg.type, msg.content.value);
    }
    return NULL;
}

// Function for the sender thread
void *sender(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = (kernel_pid_t)arg;
    msg_t msg;
    msg.type = 1; // Example message type

    for (unsigned i = 0; i < 5; ++i) {
        // Send a message to the receiver thread
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        printf("Sent message: type = %u, value = %u\n", msg.type, msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Create receiver thread
    kernel_pid_t receiver_pid = thread_create(receiver_stack,
                                         RECEIVER_STACK_SIZE,
                                         RECEIVER_PRIORITY,
                                         THREAD_CREATE_STACKTEST,
                                         receiver,
                                         NULL,
                                         "receiver");

    // Create sender thread
    thread_create(sender_stack,
                  SENDER_STACK_SIZE,
                  SENDER_PRIORITY,
                  THREAD_CREATE_STACKTEST,
                  sender,
                  (void *)receiver_pid,
                  "sender");

    return 0;
}