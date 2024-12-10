#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

// Define thread stack size and priorities
#define SENDER_STACKSIZE THREAD_STACKSIZE_MAIN
#define RECEIVER_STACKSIZE THREAD_STACKSIZE_MAIN

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN + 1)

// Define message type
#define MESSAGE_TYPE 0x1234

// Define thread stack
char sender_stack[SENDER_STACKSIZE];
char receiver_stack[RECEIVER_STACKSIZE];

// Define message queue
msg_t sender_queue[8];
msg_t receiver_queue[8];

static kernel_pid_t receiver_pid;

void *receiver(void *arg) {
    msg_t msg;
    (void)arg;
    msg_init_queue(receiver_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %lu\n", msg.content.value);
    }

    return NULL;
}

void *sender(void *arg) {
    msg_t msg;
    (void)arg;
    msg_init_queue(sender_queue, 8);

    while (1) {
        msg.type = MESSAGE_TYPE;
        msg.content.value = xtimer_now().ticks32; // sending current time as content

        printf("Sending message with content: %lu\n", msg.content.value);
        msg_send(&msg, receiver_pid);

        xtimer_sleep(2); // Sleep for 2 seconds before sending the next message
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE, RECEIVER_PRIO, THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");
    thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIO, THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    return 0;
}