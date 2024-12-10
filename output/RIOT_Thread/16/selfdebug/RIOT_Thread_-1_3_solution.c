#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "msg.h"
#include "thread.h"

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

#define SENDER_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_STACKSIZE THREAD_STACKSIZE_DEFAULT

char sender_stack[SENDER_STACKSIZE];
char receiver_stack[RECEIVER_STACKSIZE];

msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    msg_t msg;
    (void) arg;

    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %ld\n", msg.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg) {
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_t msg;

    while (1) {
        msg.content.value = random_uint32();
        printf("Sending message with content: %ld\n", msg.content.value);
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid;
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE,
                                THREAD_PRIORITY_MAIN + 1, 0, receiver_thread,
                                NULL, "receiver");

    thread_create(sender_stack, SENDER_STACKSIZE,
                  THREAD_PRIORITY_MAIN + 2, 0, sender_thread,
                  &receiver_pid, "sender");

    return 0;
}