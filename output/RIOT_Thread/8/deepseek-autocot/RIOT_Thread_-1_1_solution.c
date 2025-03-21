
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/uart.h"

#define MSG_QUEUE_SIZE 10
#define SENDER_STACK_SIZE 1024
#define RECEIVER_STACK_SIZE 1024

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1); // Send a message every second
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the sender thread with a higher priority
    sender_pid = thread_create(
        thread_stack_buffer_t(SENDER_STACK_SIZE),
        SENDER_STACK_SIZE,
        THREAD_PRIORITY_MAIN + 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    // Create the receiver thread with a lower priority
    receiver_pid = thread_create(
        thread_stack_buffer_t(RECEIVER_STACK_SIZE),
        RECEIVER_STACK_SIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    // Main thread can continue doing other tasks or just sleep
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}
