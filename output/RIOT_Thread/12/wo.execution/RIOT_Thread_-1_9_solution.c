#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define MAIN_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 3)
#define STACK_SIZE THREAD_STACKSIZE_MAIN

char sender_stack[STACK_SIZE];
char receiver_stack[STACK_SIZE];

msg_t msg_queue[8];
msg_t msg;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg.content.value = 42;
        msg.sender_pid = thread_getpid();
        printf("Sending message with value: %d\n", (int)msg.content.value);
        msg_send(&msg, thread_create(receiver, NULL, "Receiver Thread"));
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d from PID: %d\n", (int)msg.content.value, msg.sender_pid);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_stack, STACK_SIZE, SENDER_PRIO, 0, sender, NULL, "Sender Thread");
    thread_create(receiver_stack, STACK_SIZE, RECEIVER_PRIO, 0, receiver, NULL, "Receiver Thread");
    return 0;
}