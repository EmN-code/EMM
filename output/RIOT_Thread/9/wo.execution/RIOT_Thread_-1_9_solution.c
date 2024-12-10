#include <thread.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>
#include <mutex.h>
#include <ztimer.h>
#include <stdlib.h>

#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE         (THREAD_STACKSIZE_DEFAULT)

char sender_stack[THREAD_STACKSIZE];
char receiver_stack[THREAD_STACKSIZE];

msg_t msg_queue[8];
static msg_t msg;
mutex_t lock;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;

    while (1) {
        mutex_lock(&lock);
        printf("Sender: Sending message.\n");
        msg_send(&msg, thread_getpid());
        mutex_unlock(&lock);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message of type %u from thread id %u.\n", msg.type, msg.sender_pid);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER, 0, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER, 0, receiver, NULL, "receiver");

    return 0;
}