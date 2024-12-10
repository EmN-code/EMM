#include <stdio.h>  // Required for standard input and output functions
#include <msg.h>    // Required for message utilities like msg_t and mbox_t
#include <kernel.h> // Required for kernel utilities in RIOT OS like thread handling

#define MSG_QUEUE_SIZE 8

static msg_t message_queue[MSG_QUEUE_SIZE];

// Thread function to receive messages
void *message_receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(message_queue, MSG_QUEUE_SIZE);

    // Create a thread for message receiving
    static char receiver_stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t receiver_pid = thread_create(
        receiver_stack,
        sizeof(receiver_stack),
        THREAD_PRIORITY_MAIN - 1,
        0,
        message_receiver,
        NULL,
        "receiver"
    );

    // Main loop to send messages
    msg_t msg;
    int counter = 0;
    while (1) {
        msg.content.value = counter++;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return 0;
}