
#include <stdlib.h>
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <inttypes.h>

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    msg_t msg_array[MSG_QUEUE_SIZE];
    msg_init_queue(msg_array, MSG_QUEUE_SIZE);

    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, receiver_pid);
        printf("Sent message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg_array[MSG_QUEUE_SIZE];
    msg_init_queue(msg_array, MSG_QUEUE_SIZE);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    char stack[STACK_SIZE];

    receiver_pid = thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");
    sender_pid = thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 2, 0, sender_thread, NULL, "sender");

    return 0;
}
