#include <sched.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char thread_stack[THREAD_STACKSIZE];

msg_t msg_queue[4];

void *thread_function(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        // Receive message
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                               thread_function, NULL, "receiver_thread");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;  // Return error if thread creation failed
    }

    msg_init_queue(msg_queue, 4);

    msg_t msg;
    msg.type = 0xCAFE;
    // Send message to the created thread
    if (msg_send(&msg, thread_pid) < 0) {
        printf("Failed to send message\n");
        return 1;
    }

    printf("Message sent to thread PID: %d\n", thread_pid);

    return 0;
}