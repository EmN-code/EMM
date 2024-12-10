#include "msg.h"
#include "thread.h"
#include "xtimer.h"

#define SENDER_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)

char sender_stack[SENDER_STACK_SIZE];
char receiver_stack[RECEIVER_STACK_SIZE];

kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message content

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);  // Send message every second
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        // Process received message
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACK_SIZE,
                                 RECEIVER_PRIO, 0, receiver, NULL, "receiver");

    thread_create(sender_stack, SENDER_STACK_SIZE,
                  SENDER_PRIO, 0, sender, NULL, "sender");

    return 0;
}