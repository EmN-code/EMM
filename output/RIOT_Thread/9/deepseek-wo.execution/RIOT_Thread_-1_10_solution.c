
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <msg.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 16

msg_t msg_queue[NUM_MESSAGES];

void sender_thread(void *arg) {
    kernel_pid_t receiver_pid = (kernel_pid_t)(uintptr_t)arg;
    msg_t msg;
    msg.content.value = 1234;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t receiver_pid = thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        (thread_task_func_t)receiver_thread,
        NULL,
        "receiver"
    );

    thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        (thread_task_func_t)sender_thread,
        (void *)(uintptr_t)receiver_pid,
        "sender"
    );

    return 0;
}
