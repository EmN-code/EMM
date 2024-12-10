#include <xtimer.h>
#include <stdio.h>
#include <stddef.h>
#include "msg.h"
#include "thread.h"

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE           (THREAD_STACKSIZE_MAIN)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];
static msg_t msg_queue[8];
static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.type = 0x1234;
        msg_send(&msg, receiver_pid);
        printf("Sent message with type: %u\n", msg.type);
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    puts("RIOT Thread Communication Example\n");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), 
                                 RECEIVER_PRIO, 0, receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), 
                  SENDER_PRIO, 0, sender_thread, NULL, "sender");

    return 0;
}