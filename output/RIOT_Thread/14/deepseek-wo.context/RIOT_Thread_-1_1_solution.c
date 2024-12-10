
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

    while (1) {
        msg_t msg;
        msg.content.value = 0x1234; // Example message content
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

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize UART for debugging
    uart_init(UART_DEV(0), 115200, NULL, NULL);

    // Create sender thread with high priority
    thread_create(
        thread_stack_init(sender_thread, 512, NULL),
        "sender",
        PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        &sender_pid
    );

    // Create receiver thread with low priority
    thread_create(
        thread_stack_init(receiver_thread, 512, NULL),
        "receiver",
        PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        &receiver_pid
    );

    return 0;
}
