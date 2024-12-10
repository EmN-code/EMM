#include <thread.h>
#include <stdio.h>
#include "msg.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_SECOND (THREAD_PRIORITY_MAIN - 2)

static kernel_pid_t sender_thread_pid;
static kernel_pid_t receiver_thread_pid;

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        msg_send(&msg, receiver_thread_pid);
        printf("Message sent: %d\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Message received: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_thread_pid = thread_create(receiver_stack, sizeof(receiver_stack), PRIORITY_SECOND, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    if (receiver_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create receiver thread\n");
        return 1;
    }

    sender_thread_pid = thread_create(sender_stack, sizeof(sender_stack), PRIORITY_MAIN, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    if (sender_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create sender thread\n");
        return 1;
    }

    return 0;
}