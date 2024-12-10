#include <thread.h>
#include <stdio.h>

#define MSG_QUEUE_SIZE 8

static char thread_stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[MSG_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        /* Block until a message is received in the message queue */
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Creating a new thread with higher priority than the main thread */
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               0, thread_handler, NULL, "receiver");

    msg_t msg;
    msg.content.value = 42;  // Example message content

    while (1) {
        /* Send a message to the created thread */
        msg_send(&msg, thread_pid);
        printf("Sent message: %d\n", msg.content.value);

        /* Simulate some work in the main thread */
        for (volatile int i = 0; i < 1000000; ++i);
    }

    return 0;
}
