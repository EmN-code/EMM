
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdatomic.h>
#include <kernel_defines.h>

#define NUM_MESSAGES 8

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

msg_t message_queue[NUM_MESSAGES];

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send_receive(&msg, &msg, (kernel_pid_t)arg);
        printf("Sender: Message sent and received reply\n");
        sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Message received from PID %" PRIkernel_pid "\n", msg.sender_pid);
        msg_reply(&msg, &msg);
        printf("Receiver: Reply sent\n");
    }
}

int main(void) {
    msg_init_queue(message_queue, NUM_MESSAGES);

    kernel_pid_t sender_pid = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, (void *)thread_getpid(), "sender");
    kernel_pid_t receiver_pid = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    printf("Main: Threads created, sender PID: %" PRIkernel_pid ", receiver PID: %" PRIkernel_pid "\n", sender_pid, receiver_pid);

    while (1) {
        sleep(10);
    }

    return 0;
}
