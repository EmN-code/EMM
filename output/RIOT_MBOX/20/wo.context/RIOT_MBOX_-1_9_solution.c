#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <kernel.h>
#include <stdio.h>

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)

#define MESSAGE_QUEUE_SIZE  (8U)
#define INTERVAL_USEC       (1000000U) // 1 second

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t message_queue[MESSAGE_QUEUE_SIZE];

void *receiver(void *arg) {
    msg_t msg;

    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %d and content: %d\n", msg.type, msg.content.value);
    }

    return NULL;
}

void *sender(void *arg) {
    (void) arg;
    msg_t msg;
    kernel_pid_t receiver_pid = (kernel_pid_t)arg;

    while (1) {
        msg.type = 1;
        msg.content.value = 42;
        msg_send(&msg, receiver_pid);
        puts("Message sent");
        xtimer_usleep(INTERVAL_USEC);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid;

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                 receiver, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIO, THREAD_CREATE_STACKTEST,
                  sender, (void *)receiver_pid, "sender");

    return 0;
}