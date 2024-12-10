#include <stdio.h>
#include <string.h>
#include "ztimer.h"
#include "msg.h"
#include "thread.h"

#define MESSAGE_QUEUE_SIZE   (8U)
#define SENDER_SLEEP         (1U * ZTIMER_SEC)

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

void *receiver(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %d, content: %d\n", msg.type, (int)msg.content.value);
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    int count = 0;
    while (1) {
        msg.type = count % 10;
        msg.content.value = count;
        msg_send(&msg, thread_getpid());
        printf("Sent message with type: %d, content: %d\n", msg.type, count);
        ++count;
        ztimer_sleep(ZTIMER_MSEC, SENDER_SLEEP);
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    return 0;
}