
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <msg.h>
#include <thread.h>

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 10

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg) {
    msg_t msg;
    msg.content.value = 123;
    msg.type = 1;

    while (1) {
        msg_send(&msg, (kernel_pid_t)arg, false);
        sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, (void *)thread_getpid(), "sender");
    kernel_pid_t receiver_pid = thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    sender_thread((void *)receiver_pid);

    return 0;
}
