#include <thread.h>
#include <msg.h>
#include <kernel.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define MAIN_QUEUE_SIZE (8)
#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    printf("Sender: Sending message to receiver\n");
    msg_send(&msg, receiver_pid);

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t received_msg;

    while (1) {
        msg_receive(&received_msg);
        if (received_msg.type == 1) {
            printf("Receiver: Received message with value: %u\n", received_msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    receiver_pid = thread_create(receiver_thread_stack, RECEIVER_THREAD_STACKSIZE,
                                 RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    thread_create(sender_thread_stack, SENDER_THREAD_STACKSIZE,
                  SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    return 0;
}