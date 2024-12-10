#include <thread.h>
#include <stdio.h>
#include <kernel_defines.h>

// Define message queue size
#define MESSAGE_QUEUE_SIZE (8)

// Define stack sizes and priorities
#define STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_SECONDARY (THREAD_PRIORITY_MAIN - 2)

// Declare the message queue for IPC
static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

// Thread function for the main thread
void *main_thread(void *arg) {
    (void)arg; // Unused parameter
    msg_t m;

    // Initialize message queue
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

    // Loop to receive messages
    while (1) {
        if (msg_receive(&m) == 1) {
            printf("Received message with type: %u\n", (unsigned)((uint32_t)m.type));
            // Process the message here
        }
    }

    return NULL;
}

// Thread function for the secondary thread
void *secondary_thread(void *arg) {
    (void)arg; // Unused parameter
    msg_t m;
    m.type = 42;

    // Send messages in a loop
    while (1) {
        msg_send(&m, thread_getpid());
        printf("Sent message with type: %u\n", (unsigned)((uint32_t)m.type));
        // Consider adding sleep or delay to avoid spamming
    }

    return NULL;
}

int main(void) {
    char stack_main[STACKSIZE_MAIN];
    char stack_secondary[STACKSIZE_MAIN];

    kernel_pid_t pid_main;
    kernel_pid_t pid_secondary;

    // Create the main thread
    pid_main = thread_create(stack_main, sizeof(stack_main),
                             PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                             main_thread, NULL, "main");

    // Create the secondary thread
    pid_secondary = thread_create(stack_secondary, sizeof(stack_secondary),
                                  PRIORITY_SECONDARY, THREAD_CREATE_STACKTEST,
                                  secondary_thread, NULL, "secondary");

    printf("Main thread PID: %d, Secondary thread PID: %d\n", pid_main, pid_secondary);

    return 0;
}