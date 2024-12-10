#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <msg.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN (2 * THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u and value: %u\n", msg.type, msg.content.value);
    }

    return NULL;
}

void *thread_sender(void *arg) {
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while(1) {
        msg_send(&msg, receiver_pid);
        printf("Sent message with type: %u and value: %u\n", msg.type, msg.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    kernel_pid_t receiver_thread = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "receiver");
    
    kernel_pid_t sender_thread = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_sender, &receiver_thread, "sender");

    thread_yield();

    return 0;
}
