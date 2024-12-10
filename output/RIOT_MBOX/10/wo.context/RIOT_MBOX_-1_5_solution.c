// Description of the problem and approach
// The task is to implement a message box functionality using RIOT on an ESP32, with events sent at intervals. However, we encountered a compilation error due to a missing directory or file reference in the include directive: "event/event.h".
// To solve the error, we need to include the correct header files that are available. We will adjust the file paths according to the available headers.

// Include necessary headers in descending alphabetical order
#include <stdio.h>
#include <msg.h>  // Correct header file for using message queues in RIOT

// Define stack size for message thread
#define THREAD_STACKSIZE_DEFAULT    (1024U)

// Create a message queue
static msg_t msg_queue[8];

// Thread function to process messages
void *message_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        // Wait for a message
        msg_receive(&msg);
        // Process the message (In this case, just print it)
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

// Main function
int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, 8);

    // Create a thread for message processing
    kernel_pid_t msg_thread_pid = thread_create(
        (char[THREAD_STACKSIZE_DEFAULT]){},
        THREAD_STACKSIZE_DEFAULT, 
        THREAD_PRIORITY_MAIN - 1, 
        THREAD_CREATE_STACKTEST, 
        message_thread, 
        NULL, 
        "message_thread"
    );

    msg_t msg;
    int msg_counter = 0;
    while (1) {
        // Send a message every second
        msg.content.value = msg_counter++;
        msg_send(&msg, msg_thread_pid);
        xtimer_sleep(1);
    }

    return 0;
}