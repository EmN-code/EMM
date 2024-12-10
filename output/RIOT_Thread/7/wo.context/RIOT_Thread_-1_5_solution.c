#include "thread.h"
#include "xtimer.h"
#include <stdio.h>
#include "msg.h"

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

#define SENDER_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_STACKSIZE THREAD_STACKSIZE_DEFAULT

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int count = 0;
    
    while (1) {
        msg.content.value = count++;
        printf("Sender: Sending message with value %d\n", msg.content.value);
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    char sender_stack[SENDER_STACKSIZE];
    char receiver_stack[RECEIVER_STACKSIZE];
    
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE,
                                 RECEIVER_THREAD_PRIORITY, 0,
                                 receiver_thread, NULL, "receiver");
    
    thread_create(sender_stack, SENDER_STACKSIZE,
                  SENDER_THREAD_PRIORITY, 0,
                  sender_thread, NULL, "sender");

    return 0;
}