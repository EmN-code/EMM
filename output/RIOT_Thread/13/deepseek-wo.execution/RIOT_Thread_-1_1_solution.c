
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/uart.h"

#define MSG_QUEUE_SIZE 10
#define PRIORITY_HIGH 1
#define PRIORITY_LOW 5

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1); // Send a message every second
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    uart_init(UART_DEV(0), 115200, UART_FLAG_NONE);

    thread_create(
        stack_buffer, sizeof(stack_buffer),
        PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL, "sender");

    thread_create(
        stack_buffer, sizeof(stack_buffer),
        PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL, "receiver");

    sender_pid = thread_getpid();
    receiver_pid = thread_getpid();

    return 0;
}