#include <thread.h>
#include <stdio.h>
#include <stdint.h>
#include <msg.h>

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE           (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE      (8)

char sender_stack[STACKSIZE];
char receiver_stack[STACKSIZE];
msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message
    
    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Message sent: %lu\n", (uint32_t)msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Message received: %lu\n", (uint32_t)msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, STACKSIZE,
                                               RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                               receiver, NULL, "receiver");

    thread_create(sender_stack, STACKSIZE, SENDER_PRIO, THREAD_CREATE_STACKTEST,
                  sender, &receiver_pid, "sender");

    return 0;
}