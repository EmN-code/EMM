
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define SENDER_STACK_SIZE 1024
#define RECEIVER_STACK_SIZE 1024

kernel_pid_t sender_pid, receiver_pid;
msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 123; // Example message content

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
    thread_create(
        stack_buffer, SENDER_STACK_SIZE,
        THREAD_PRIORITY_MAIN + 1,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    // Create the receiver thread with a lower priority
    thread_create(
        stack_buffer, RECEIVER_STACK_SIZE,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    return 0;
}
