
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <ztimer.h>
#include <timex.h>
#include <event/thread.h>
#include <xtimer.h>

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        msg_send(&msg, receiver_pid);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void) {
    char sender_stack[STACK_SIZE];
    char receiver_stack[STACK_SIZE];

    sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    return 0;
}
