#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_THREAD_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define MESSAGE_QUEUE_SIZE (8)

// Thread stacks
static char sender_thread_stack[SENDER_THREAD_STACK_SIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACK_SIZE];

// Message queue for the receiver thread
static msg_t receiver_msg_queue[MESSAGE_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message content
    while (1) {
        printf("Sender: Sending message...\n");
        msg_send(&msg, thread_getpid_of((thread_t *)receiver_thread_stack));
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(receiver_msg_queue, MESSAGE_QUEUE_SIZE);
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, RECEIVER_THREAD_STACK_SIZE,
                              THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                              receiver_thread, NULL, "receiver_thread");

    thread_create(sender_thread_stack, SENDER_THREAD_STACK_SIZE,
                  THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender_thread");

    printf("Main: Threads started, running indefinitely.\n");
    return 0;
}