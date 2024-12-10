#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE (8)

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 20;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);
        printf("Sender: Sending message.\n");
        msg_send(&msg, thread_getpid());
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with type %u and value %u.\n", msg.type, msg.content.value);
    }

    return NULL;
}

int main(void) {
    thread_create(receiver_thread_stack, RECEIVER_THREAD_STACKSIZE, RECEIVER_THREAD_PRIORITY, 0,
                  receiver_thread, NULL, "receiver");
    thread_create(sender_thread_stack, SENDER_THREAD_STACKSIZE, SENDER_THREAD_PRIORITY, 0,
                  sender_thread, NULL, "sender");

    return 0;
}