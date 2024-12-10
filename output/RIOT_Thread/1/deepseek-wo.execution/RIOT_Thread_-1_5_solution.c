
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define STACKSIZE (1024)
#define NUM_MESSAGES (8)

msg_t msg_queue[NUM_MESSAGES];

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send_receive(&msg, &msg, (kernel_pid_t)arg);
        printf("Sender: Sent message with value %lu\n", msg.content.value);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t receiver_pid = thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        (void *)receiver_pid,
        "sender"
    );

    return 0;
}
