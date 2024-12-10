#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE           (8)

static char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    msg_t msg;
    msg_init_queue(msg.sender_array, MSG_QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    msg_t msg;
    msg.type = 0x1234;
    for (int i = 0; i < 5; i++) {
        msg.content.value = i;
        if (msg_send(&msg, receiver_pid) < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent: %d\n", i);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(NULL, 0);  // Initialize default receiver

    receiver_pid = thread_create(
        receiver_thread_stack, sizeof(receiver_thread_stack),
        THREAD_PRIORITY_RECEIVER, 0, receiver_thread,
        NULL, "receiver thread");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(
        sender_thread_stack, sizeof(sender_thread_stack),
        THREAD_PRIORITY_SENDER, 0, sender_thread,
        NULL, "sender thread");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create sender thread\n");
        return 1;
    }

    return 0;
}