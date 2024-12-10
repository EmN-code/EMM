#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "msg.h"

char sender_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_stack[THREAD_STACKSIZE_DEFAULT];

void *sender_thread(void *arg) {
    msg_t msg;
    msg.content.value = 42;  // example content

    while (1) {
        msg_send(&msg, *(kernel_pid_t*)arg);
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_MAIN - 1, 0, receiver_thread,
                                              NULL, "receiver");
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_MAIN - 2, 0, sender_thread,
                                            &receiver_pid, "sender");

    (void)sender_pid;  // To quiet unused variable warning if necessary

    return 0;
}