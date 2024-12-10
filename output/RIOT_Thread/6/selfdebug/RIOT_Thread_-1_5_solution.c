#include "thread.h"
#include "msg.h"
#include "stdio.h"

#define SENDER_STACKSIZE   (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    printf("Sender: Sending message to receiver\n");
    msg.content.value = 42;  // arbitrary value
    msg_send(&msg, thread_getpid_of(thread_get(RECEIVER_PRIO)));
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_stack, RECEIVER_STACKSIZE, RECEIVER_PRIO, 0,
                  receiver_thread, NULL, "receiver");
    thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIO, 0,
                  sender_thread, NULL, "sender");

    return 0;
}