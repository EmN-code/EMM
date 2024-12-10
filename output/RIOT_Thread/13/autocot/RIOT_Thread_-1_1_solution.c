#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define MSG_QUEUE_SIZE     (8)
#define THREAD_STACKSIZE   (THREAD_STACKSIZE_MAIN)

char sender_thread_stack[THREAD_STACKSIZE];
char receiver_thread_stack[THREAD_STACKSIZE];

msg_t sender_msg_queue[MSG_QUEUE_SIZE];
msg_t receiver_msg_queue[MSG_QUEUE_SIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    msg_t msg;
    (void)arg;
    for (int i = 0; i < 5; i++) {
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        printf("Sent message %d\n", i);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(sender_msg_queue, MSG_QUEUE_SIZE);
    msg_init_queue(receiver_msg_queue, MSG_QUEUE_SIZE);

    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                 THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");
                                 
    sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                               THREAD_PRIORITY_MAIN - 2, 0, sender_thread, NULL, "sender");

    return 0;
}