#include <stdio.h>
#include <thread
#include <msg

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_IDLE + 1)
#define THREAD_PRIORITY_HIGH  (THREAD_PRIORITY_MAIN - 1)

char thread_stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8]; // Message queue for incoming messages

void *receiver_thread(void *arg) {
    msg_t msg;
    // Initialize message queue
    msg_init_queue(msg_queue, 8);

    while (1) {
        // Blocking wait for a message
        msg_receive(&msg);
        printf("Received message with content: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT application\n");

    // Start receiver thread with higher priority
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_HIGH,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    msg_t msg;
    msg.content.value = 42; // Data to send

    // Sending a message to receiver thread
    msg_send_receive(&msg, NULL, thread_getpid());

    puts("Finished sending message\n");

    return 0;
}