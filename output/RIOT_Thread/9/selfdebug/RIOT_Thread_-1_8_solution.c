#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>

#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 2)
#define STACKSIZE                THREAD_STACKSIZE_MAIN
#define MSG_QUEUE_SIZE           8

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 100; // Example message content
    while (1) {
        printf("Sender: Sending message.\n");
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Message received with value %lu.\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), 
                                 THREAD_PRIORITY_RECEIVER, 0, receiver_thread, 
                                 NULL, "receiver");
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), 
                               THREAD_PRIORITY_SENDER, 0, sender_thread, 
                               NULL, "sender");
    return 0;
}