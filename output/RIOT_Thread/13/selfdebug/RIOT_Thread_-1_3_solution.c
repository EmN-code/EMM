#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE        THREAD_STACKSIZE_MAIN

static char sender_thread_stack[THREAD_STACKSIZE];
static char receiver_thread_stack[THREAD_STACKSIZE];

msg_t msg_queue[32];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;
    msg.content.value = 42;
    kernel_pid_t receiver_pid = thread_getpid();  // Get PID of receiver

    if (msg_send_receive(&msg, &reply, receiver_pid) == 1) {
        printf("Message sent and acknowledged with reply: %d\n", reply.content.value);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Message received with value: %lu\n", (unsigned long)msg.content.value);
        msg.content.value += 1;
        msg_reply(&msg, &msg.content);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 32);

    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                            sender, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                              THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                              receiver, NULL, "receiver");

    if (sender_pid <= KERNEL_PID_UNDEF || receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    return 0;
}