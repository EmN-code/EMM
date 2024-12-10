#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO        (THREAD_PRIORITY_MAIN + 1)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char sender_thread_stack[THREAD_STACKSIZE_MAIN];
static char receiver_thread_stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = 42;
    printf("Sender: Preparing to send message with value: %d\n", msg.content.value);
    msg_send(&msg, thread_get(RECEIVER_PRIO));
    printf("Sender: Message sent\n");
    return NULL;
}

void *receiver(void *arg) {
    (void) arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Receiver: Received message with value: %d\n", msg.content.value);
    return NULL;
}

int main(void) {
    printf("RIOT message passing example with different thread priorities\n");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  SENDER_PRIO, THREAD_CREATE_STACKTEST,
                  sender, NULL, "sender");

    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                  receiver, NULL, "receiver");

    return 0;
}