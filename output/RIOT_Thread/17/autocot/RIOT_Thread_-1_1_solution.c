#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)

msg_t queue[8];

void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg; // To suppress unused variable warning

    while (1) {
        /* Wait for a message */
        msg_receive(&msg);
        printf("Message received with content: %ld\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    msg_t msg;
    kernel_pid_t receiver_pid = *((kernel_pid_t*)arg);

    for (int i = 0; i < 5; i++) {
        msg.content.value = i;
        /* Send a message to the receiver */
        msg_send(&msg, receiver_pid);
        printf("Message sent with content: %d\n", i);
        /* Simulate some work */
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    kernel_pid_t receiver_pid = thread_create(receiver_stack, STACK_SIZE,
                                              PRIORITY_LOW, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    char sender_stack[STACK_SIZE];
    thread_create(sender_stack, STACK_SIZE,
                  PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
                  sender_thread, &receiver_pid, "sender");

    return 0;
}