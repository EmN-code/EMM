#include <thread.h>
#include <msg.h>
#include <stdio.h>

// Define stack size and message queue size
#define THREAD_STACKSIZE_LARGE (THREAD_STACKSIZE_MAIN)
#define MESSAGE_QUEUE_SIZE     (8)

// Function declarations
void *message_sender(void *arg);
void *message_receiver(void *arg);

// Declare message queue and sender PID
static msg_t message_queue[MESSAGE_QUEUE_SIZE];
kernel_pid_t sender_pid;

// Message sender thread function
void *message_sender(void *arg) {
    (void)arg; // Unused parameter
    msg_t msg;
    msg.content.value = 42; // Example message content
    
    while (1) {
        msg_send(&msg, sender_pid);
        puts("Message sent.");
        xtimer_sleep(1); // Delay for demonstration
    }
    return NULL;
}

// Message receiver thread function
void *message_receiver(void *arg) {
    (void)arg; // Unused parameter
    msg_t msg;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", (unsigned)msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);
    
    // Thread configurations
    char sender_stack[THREAD_STACKSIZE_LARGE];
    char receiver_stack[THREAD_STACKSIZE_LARGE];

    // Create message receiver thread
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_MAIN - 1, 0,
                                              message_receiver, NULL,
                                              "receiver");

    // Assign receiver_pid to sender_pid
    sender_pid = receiver_pid;

    // Create message sender thread
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_MAIN - 2, 0,
                  message_sender, NULL,
                  "sender");

    return 0;
}