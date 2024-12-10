
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/uart.h"

#define THREAD_STACKSIZE_SENDER   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_RECEIVER (THREAD_STACKSIZE_DEFAULT)

#define PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 123; // Example message content
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
    // Initialize UART for debugging
    uart_init(UART_DEV(0), 115200, NULL, NULL);

    // Create sender thread
    thread_create(
        thread_stack_init(sender_thread, THREAD_STACKSIZE_SENDER, NULL),
        THREAD_STACKSIZE_SENDER,
        PRIORITY_SENDER,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender");

    // Create receiver thread
    thread_create(
        thread_stack_init(receiver_thread, THREAD_STACKSIZE_RECEIVER, NULL),
        THREAD_STACKSIZE_RECEIVER,
        PRIORITY_RECEIVER,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver");

    // Get PIDs of the created threads
    sender_pid = thread_getpid();
    receiver_pid = thread_getpid();

    // Main thread will just sleep to let the other threads run
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}
