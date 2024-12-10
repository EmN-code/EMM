#include <stdio.h>  // Added for printf declaration
#include <stdlib.h>
#include <sched.h>
#include <net/sock/util.h>
#include <sys/types.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;
char receiver_stack[THREAD_STACKSIZE_MAIN];
char sender_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value
    printf("Sender: Sending message with value %d\n", msg.content.value);
    msg_send(&msg, receiver_pid);
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", msg.content.value);
    }
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "Receiver");
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, sender_thread, NULL, "Sender");
    return 0;
}