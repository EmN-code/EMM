
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <uart.h>

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

char stack_buffer[STACK_SIZE];
msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg) {
    msg_t msg;
    msg.content.value = 123;
    msg.type = 1;

    while (1) {
        msg_send(&msg, (kernel_pid_t)arg, false);
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
    kernel_pid_t sender_pid, receiver_pid;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    receiver_pid = thread_create(stack_buffer, STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST, (thread_task_func_t)receiver_thread,
                                 NULL, "receiver");

    sender_pid = thread_create(stack_buffer, STACK_SIZE, THREAD_PRIORITY_MAIN - 2,
                               THREAD_CREATE_STACKTEST, (thread_task_func_t)sender_thread,
                               (void *)receiver_pid, "sender");

    uart_init(UART_DEV(0), 115200, UART_PARITY_NONE);

    return 0;
}
