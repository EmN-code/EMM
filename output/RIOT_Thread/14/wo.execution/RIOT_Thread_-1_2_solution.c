#include <thread.h>
#include <msg.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <stdio.h>

// Define the stack sizes and priorities for the threads
#define THREAD_STACKSIZE_MAIN    1024
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 3)

// Declare thread stacks
char receiver_stack[THREAD_STACKSIZE_MAIN];
char sender_stack[THREAD_STACKSIZE_MAIN];

msg_t msg;
msg_t reply;

// Function for the receiver thread
void *receiver_thread(void *arg) {
    (void)arg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
        // Sending a reply
        reply.content.value = msg.content.value + 1; // Increment received value
        msg_reply(&msg, &reply);
    }
    return NULL;
}

// Function for the sender thread
void *sender_thread(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    for (uint32_t i = 0; i < 10; i++) {
        // Prepare message
        msg.content.value = i;
        if (msg_send_receive(&msg, &reply, receiver_pid) >= 0) {
            printf("Sent message with value: %u, received reply with value: %u\n", i, reply.content.value);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Create the receiver thread
    kernel_pid_t receiver_pid = thread_create(
        receiver_stack, sizeof(receiver_stack),
        THREAD_PRIORITY_RECEIVER, 0,
        receiver_thread, NULL, "receiver");

    // Create the sender thread
    thread_create(
        sender_stack, sizeof(sender_stack),
        THREAD_PRIORITY_SENDER, 0,
        sender_thread, &receiver_pid, "sender");

    return 0;
}