
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 1024
#define MSG_QUEUE_SIZE 8

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    msg_t msg_array[MSG_QUEUE_SIZE];
    msg_t msg, reply;

    msg_init_queue(msg_array, MSG_QUEUE_SIZE);

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        msg.type = 1;
        msg_send_receive(&msg, &reply, receiver_pid);
        printf("Sender: Received reply %d\n", reply.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg_array[MSG_QUEUE_SIZE];
    msg_t msg, reply;

    msg_init_queue(msg_array, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message %d\n", msg.content.value);
        reply.content.value = msg.content.value * 2;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void) {
    char sender_stack[STACKSIZE];
    char receiver_stack[STACKSIZE];

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 1, 0,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN - 2, 0,
                               sender_thread, NULL, "sender");

    return 0;
}
