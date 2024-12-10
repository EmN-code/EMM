#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static msg_t sender_queue[8];
static msg_t receiver_queue[8];

// Function for the receiver thread
void *receiver(void *arg) {
    msg_t msg;
    (void)arg;

    msg_init_queue(receiver_queue, 8);
    
    while (1) {
        msg_receive(&msg);
        printf("Receiver received a message: %d\n", msg.content.value);
    }
    return NULL;
}

// Function for the sender thread
void *sender(void *arg) {
    msg_t msg;
    (void)arg;

    msg.content.value = 42;
    msg_send(&msg, thread_getpid_of((thread_t *)arg));
    printf("Sender sent a message.\n");

    return NULL;
}

int main(void) {
    thread_t sender_tid;
    thread_t receiver_tid;

    // Create receiver thread
    receiver_tid = thread_create(receiver_stack, RECEIVER_STACKSIZE,
                                 RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                 receiver, NULL, "receiver");

    // Create sender thread, passing receiver thread ID to it
    sender_tid = thread_create(sender_stack, SENDER_STACKSIZE,
                               SENDER_PRIO, THREAD_CREATE_STACKTEST,
                               sender, (void *)receiver_tid, "sender");

    // Let the threads run
    while (1) {}

    return 0;
}