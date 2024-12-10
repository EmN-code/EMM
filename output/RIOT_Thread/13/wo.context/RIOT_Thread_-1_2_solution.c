#include <thread.h>
#include <stdio.h>

// Define priorities for the threads
#define THREAD_PRIORITY_RECEIVER       (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER         (THREAD_PRIORITY_MAIN - 2)

// Declare stacks for threads
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
char receiver_thread_stack[STACKSIZE];
char sender_thread_stack[STACKSIZE];

// Declare message queue
#define QUEUE_SIZE (8)
msg_t msg_queue[QUEUE_SIZE];

static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg; // unused

    // Initialize message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    while (1) {
        // Receive a message
        if (msg_receive(&msg) < 0) {
            printf("Error receiving message!\n");
            continue;
        }
        printf("Received message with content: %ld\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    msg_t msg;
    (void)arg; // unused

    while (1) {
        // Prepare the message
        msg.content.value = (uint32_t)rand();

        // Send message to the receiver
        if (msg_send(&msg, receiver_pid) < 0) {
            printf("Error sending message!\n");
        } else {
            printf("Sent message with content: %ld\n", msg.content.value);
        }
        
        // Sleep for some time before sending next message
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Create receiver thread
    receiver_pid = thread_create(receiver_thread_stack, STACKSIZE,
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver_thread, NULL, "receiver_thread");

    // Create sender thread
    thread_create(sender_thread_stack, STACKSIZE,
                  THREAD_PRIORITY_SENDER, 0,
                  sender_thread, NULL, "sender_thread");

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}