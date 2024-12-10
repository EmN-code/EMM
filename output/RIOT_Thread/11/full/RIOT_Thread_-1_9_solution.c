#include "kernel.h"

#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE 1024

char sender_stack[STACKSIZE];
char receiver_stack[STACKSIZE];
msg_t msg_queue[8];

void *receiver(void *arg) {
    msg_t msg;
    int cnt = 0;
    while (1) {
        msg_receive(&msg);
        printf("Received Message #%d with Type: %d\n", cnt++, msg.type);
    }

    return NULL;
}

void *sender(void *arg) {
    msg_t msg;
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    int cnt = 0;

    while (1) {
        msg.type = cnt % UINT16_MAX;
        msg_send(&msg, receiver_pid);
        printf("Sent Message #%d with Type: %d\n", cnt, msg.type);
        cnt++;
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIORITY, 0, receiver, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIORITY, 0, sender, &receiver_pid, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }

    return 0;
}