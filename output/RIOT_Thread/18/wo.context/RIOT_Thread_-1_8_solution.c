#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE_MAIN      (THREAD_STACKSIZE_MAIN)

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

static kernel_pid_t receiver_pid;

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) < 0) {
            printf("Error receiving message\n");
        } else {
            printf("Received message with value: %ld\n", msg.content.value);
        }
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Sample message content

    while (1) {
        printf("Sending message with value: %ld\n", msg.content.value);
        if (msg_send(&msg, receiver_pid) < 0) {
            printf("Error sending message\n");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_PRIO, 0,
                                 receiver, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            SENDER_PRIO, 0,
                                            sender, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }
    return 0;
}