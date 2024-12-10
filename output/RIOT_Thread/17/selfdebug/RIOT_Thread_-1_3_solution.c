#include <stdio.h>
#include <msg.h>
#include <xtimer.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define QUEUE_SIZE    (8)

msg_t queue[QUEUE_SIZE];

char stack_sender[THREAD_STACKSIZE_DEFAULT];
char stack_receiver[THREAD_STACKSIZE_DEFAULT];
kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg.content.value = 42; // Example message
        printf("Sender: Sending message to receiver\n");
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", (int)msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                 RECEIVER_PRIO, 0, receiver,
                                 NULL, "receiver");
    thread_create(stack_sender, sizeof(stack_sender),
                  SENDER_PRIO, 0, sender,
                  NULL, "sender");
    return 0;
}