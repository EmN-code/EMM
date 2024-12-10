#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define SENDER_PRIO          (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO        (THREAD_PRIORITY_MAIN - 2)
#define MESSAGE_QUEUE_SIZE   (8)

static msg_t sender_msg_queue[MESSAGE_QUEUE_SIZE];
static msg_t receiver_msg_queue[MESSAGE_QUEUE_SIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;
    while(1) {
        msg.content.value = counter++;
        msg_send(&msg, receiver_pid);
        printf("Sender: Sent message with value %d\n", msg.content.value);
        thread_yield_higher();  // Yield to allow receiver to process message
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(receiver_msg_queue, MESSAGE_QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(sender_msg_queue, MESSAGE_QUEUE_SIZE);

    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    printf("Threads have been created and started\n");

    return 0;
}