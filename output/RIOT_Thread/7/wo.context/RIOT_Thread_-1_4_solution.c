#include "msg.h"
#include "kernel_types.h"
#include "thread.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[8];

kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %d\n", msg.type);
    }

    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 10;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 1, 0,
                                 receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_MAIN, 0,
                  sender_thread, NULL, "sender");

    return 0;
}