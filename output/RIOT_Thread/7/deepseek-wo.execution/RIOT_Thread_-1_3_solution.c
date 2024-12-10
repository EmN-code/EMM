
#include <stdio.h>
#include <stdint.h>
#include <kernel_internals.h>
#include <thread.h>
#include <msg.h>

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

char thread_stack_1[STACK_SIZE];
char thread_stack_2[STACK_SIZE];

msg_t msg_queue_1[MSG_QUEUE_SIZE];
msg_t msg_queue_2[MSG_QUEUE_SIZE];

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 12345;

    while (1) {
        msg_send(&msg, thread_getpid());
        printf("Sent message with value: %lu\n", (unsigned long)msg.content.value);
        thread_sleep(1000000); // Sleep for 1 second
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue_1, MSG_QUEUE_SIZE);
    msg_init_queue(msg_queue_2, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(thread_stack_1, STACK_SIZE, PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(thread_stack_2, STACK_SIZE, PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
