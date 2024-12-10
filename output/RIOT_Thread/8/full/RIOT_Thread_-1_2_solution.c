#include "thread.h"
#include "time_units.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "msg.h"
#include "event/thread.h"

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN + 1)

static msg_t msg_queue[8];

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;
    int res;

    while (1) {
        res = msg_send(&msg, receiver_pid);
        if (res == 1) {
            printf("Message sent successfully\n");
        } else {
            printf("Failed to send message\n");
        }
        sleep(1);
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return NULL;
}


int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_PRIO, 0, receiver, NULL, "Receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_PRIO, 0, sender, NULL, "Sender");

    while (1) {
        // Keep the main thread alive
        sleep(10);
    }
    return 0;
}