/*
Multi-threading and message passing example in RIOT OS for ESP32, including inter-thread communication using msg_t structure.
To handle inter-thread communication, two threads are created. One will send a message to the other, which receives and processes it.
Make sure all necessary components and the correct RIOT version are installed and compatible with your ESP32 board.
*/

#include "netinet/in.h"
#include "net/gcoap.h" // Placeholder to simulate required header inclusion
#include "net/sock/util.h" // Placeholder to simulate required header inclusion
#include "net/emcute.h" // Placeholder to simulate required header inclusion

// Thread function signatures
void *sender_thread(void *arg);
void *receiver_thread(void *arg);

// Global variables for PIDs
kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

// Allocate stacks for the threads
char sender_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_stack[THREAD_STACKSIZE_DEFAULT];

// Message queues
msg_t msg_array_1[8]; // Receiver queue
msg_t msg_array_2[8]; // Sender queue (although not needed for simple send-only)

// Main function that initializes threads and message queues
int main(void) {
    // Initialize message queues
    msg_init_queue(msg_array_1, 8);
    msg_init_queue(msg_array_2, 8);

    // Create sender thread
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN-1,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    // Create receiver thread
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN-2,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}

// Thread function that sends a message
void *sender_thread(void *arg) {
    (void) arg; // Unused
    msg_t msg;
    msg.content.value = 42; // Dummy content
    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2); // 2 seconds delay between sends
    }
    return NULL;
}

// Thread function that receives a message
void *receiver_thread(void *arg) {
    (void) arg; // Unused
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}