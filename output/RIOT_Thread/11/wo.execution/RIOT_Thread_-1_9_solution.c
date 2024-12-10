# Description: This code demonstrates inter-thread communication in RIOT on an ESP32, with one thread sending messages to another using `msg_t` structure.

#include "xtimer.h" // For timer functions
#include "thread.h" // For thread management
#include "msg.h" // For message management

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECONDARY (THREAD_PRIORITY_MAIN + 1)

// Message structures
msg_t msg_queue[4];
msg_t send_msg;

// Thread stacks
char thread_stack[THREAD_STACKSIZE_DEFAULT];

// Function for the main thread
void *receive_thread(void *arg) {
    while (1) {
        msg_t msg;
        msg_receive(&msg);  // Blocking wait for any message
        printf("Received message: Type - %u, Value - %lu\n", msg.type, msg.content.value);
    }
    return NULL;
}

// Function for the secondary thread
void *send_thread(void *arg) {
    (void)arg;
    xtimer_sleep(2); // Give some time for setup

    send_msg.type = 1;
    send_msg.content.value = 42;

    // Send the message
    msg_send(&send_msg, thread_getpid());

    return NULL;
}

int main(void) {
    // Initialize the message queue  
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    // Create the secondary thread
    kernel_pid_t receiver_pid = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN,
        0, receive_thread, NULL, "receiver thread");

    // Call the sender function
    send_thread(NULL);

    return 0;
}
