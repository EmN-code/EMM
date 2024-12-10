#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)

/* Message Queue for the Receiver Thread */
msg_t rcv_queue[8];

void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg;
    msg_init_queue(rcv_queue, 8);
    printf("Receiver thread started. Waiting for messages...\n");
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u and value: %u\n", msg.type, msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_t msg;
    printf("Sender thread started. Sending messages...\n");
    for (uint16_t i = 0; i < 10; ++i) {
        msg.type = i;
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        printf("Sent message with type: %u and value: %u\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid;
    char receiver_stack[RECEIVER_STACKSIZE];
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE, RECEIVER_PRIO, THREAD_CREATE_SLEEPING, receiver_thread, NULL, "receiver");

    char sender_stack[SENDER_STACKSIZE];
    thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIO, THREAD_CREATE_SLEEPING, sender_thread, &receiver_pid, "sender");

    thread_wakeup(receiver_pid);

    return 0;
}