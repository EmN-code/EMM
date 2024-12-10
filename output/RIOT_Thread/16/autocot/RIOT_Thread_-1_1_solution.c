#include "thread.h"
#include "msg.h"
#include "kernel_types.h"
#include <stdio.h>

#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN + 1)
#define STACKSIZE                (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE           (8)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *receiver_thread(void *arg) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver thread: Received message: %d\n", msg.content.value);
        }
    }
    return NULL;
}

void *sender_thread(void *arg) {
    kernel_pid_t receiver_pid = (kernel_pid_t)arg;
    msg_t msg;
    msg.type = 1;
    
    for (int i = 0; i < 5; i++) {
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        printf("Sender thread: Sent message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid =
        thread_create(receiver_stack, STACKSIZE,
                      RECEIVER_THREAD_PRIORITY,
                      THREAD_CREATE_STACKTEST,
                      receiver_thread, NULL, "receiver");
    
    thread_create(sender_stack, STACKSIZE,
                  SENDER_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST,
                  sender_thread, (void *)receiver_pid, "sender");

    return 0;
}