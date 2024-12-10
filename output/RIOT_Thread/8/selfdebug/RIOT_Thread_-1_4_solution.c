#include <thread.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

// Message type
#define MESSAGE_TYPE (0x1234)

// Two threads are used in this example to achieve inter-thread communication using msg_t.
char stack[THREAD_STACKSIZE_MAIN];
msg_t queue[8]; // Message queue for receiving thread

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(queue, 8); // Initialize the message queue

    printf("Receiver thread: Ready to receive messages.\n");

    while (1) {
        // Block until a message was received
        msg_receive(&msg);
        printf("Receiver thread: Received message with type %u and value %u.\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Create a thread
    kernel_pid_t thread_pid = thread_create(stack,
                                            sizeof(stack),
                                            THREAD_PRIORITY_MAIN,
                                            0, 
                                            thread_handler,
                                            NULL,
                                            "receiver_thread");

    printf("Main thread: Created receiver thread with PID %d.\n", thread_pid);

    msg_t msg;
    msg.type = MESSAGE_TYPE;

    while (1) {
        printf("Main thread: Preparing to send message.\n");
        msg.content.value = 100;

        // Send a message to the receiver thread
        msg_send(&msg, thread_pid);
        printf("Main thread: Message sent with value %u.\n", msg.content.value);

        // Sleep for a while before sending the next message
        xtimer_sleep(2);
    }

    return 0;
}