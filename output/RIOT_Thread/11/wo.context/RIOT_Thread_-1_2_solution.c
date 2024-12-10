#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_THREAD_STACKSIZE];
static char receiver_stack[RECEIVER_THREAD_STACKSIZE];

static msg_t queue[8];
static msg_t msg;

void *sender(void *arg) {
    (void)arg;
    while (1) {
        printf("Sender: Sending message\n");
        msg.content.value = 42; // Just an example content
        msg_send(&msg, thread_get(RECEIVER_THREAD_PRIORITY));
        xtimer_sleep(2); // Ensure "xtimer_sleep" is properly included
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(queue, 8);

    while (1) {
        if (msg_receive(&msg)) {
            printf("Receiver: Received message with value %ld\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_stack, RECEIVER_THREAD_STACKSIZE, RECEIVER_THREAD_PRIORITY, 0, receiver, NULL, "receiver");
    thread_create(sender_stack, SENDER_THREAD_STACKSIZE, SENDER_THREAD_PRIORITY, 0, sender, NULL, "sender");

    return 0;
}