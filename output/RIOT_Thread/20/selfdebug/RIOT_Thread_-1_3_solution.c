#include <thread.h> 
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

static char thread_stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg) {
    msg_t msg;

    while (1) {
        // Receiving message
        msg_receive(&msg);
        printf("Received message of type: %d\n", msg.type);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    msg_t msg;

    // Create thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, thread_handler, NULL, "recv_thread");

    // Sending a message
    msg.content.value = 42;  // Assign some value
    msg.type = 1;  // Define the message type

    msg_send_receive(&msg, &msg, thread_pid);

    return 0;
}