#include <stdio.h> // Necessary for printf
#include <msg.h>  // Includes the messaging API for RIOT
#include <kernel.h> // Provides threading and kernel functions

#define STACKSIZE THREAD_STACKSIZE_DEFAULT
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue_sender[MSG_QUEUE_SIZE];
static msg_t msg_queue_receiver[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        msg_send(&msg, thread_get(2)); // Retrieve PID for receiver thread
        printf("Message sent from sender with content: %d\n", msg.content.value);
        xtimer_sleep(1); // Sleep for 1 second
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue_receiver, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Message received with content: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    static char sender_stack[STACKSIZE];
    static char receiver_stack[STACKSIZE];

    kernel_pid_t pid_sender = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                            sender_thread, NULL, "sender");

    kernel_pid_t pid_receiver = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    // Suppressing unused variable warnings
    (void)pid_sender;
    (void)pid_receiver;

    return 0;
}