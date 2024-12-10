#include <xtimer.h>  // Timer functions
#include <thread.h>  // Threading
#include <stdio.h>   // Standard I/O for printf

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)

static char thread_stack[THREAD_STACKSIZE_MAIN]; // Stack for the new thread

// Define a message queue with a power of two size
enum {MSG_QUEUE_SIZE = 4};
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;

    // Initialize the message queue for this thread
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with content: %ld\n", msg.content.value);
        } else {
            printf("Failed to receive message.\n");
        }
    }

    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;

    // Create a new thread with a higher priority than MAIN thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_handler,
                               NULL, "receiver");

    if (thread_pid > 0) {
        printf("Thread started successfully\n");
    } else {
        printf("Thread creation failed\n");
        return 1;
    }

    // Sending messages
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        if (msg_try_send(&msg, thread_pid) != 1) {
            printf("Failed to send message\n");
            xtimer_sleep(1);
        } else {
            printf("Message sent: %ld\n", msg.content.value);
            xtimer_sleep(2);
        }
    }

    return 0;
}