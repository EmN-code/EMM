
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE 512
#define MSG_QUEUE_SIZE 10

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        // Prepare the message
        msg.content.value = 123; // Example value
        msg.sender_pid = thread_getpid();

        // Send the message to the receiver thread
        msg_send(&msg, receiver_pid);

        // Sleep for a while before sending the next message
        xtimer_sleep(2);
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

        // Wait for a message
        msg_receive(&msg);

        // Process the received message
        printf("Received message with value: %d from thread %d\n", msg.content.value, msg.sender_pid);
    }

    return NULL;
}

int main(void)
{
    // Initialize the GPIOs (example initialization)
    gpio_init(GPIO_PIN(0, 1), GPIO_OUT);

    // Create the sender thread with higher priority
    sender_pid = thread_create(
        stack_buffer, THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN + 1,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    // Create the receiver thread with lower priority
    receiver_pid = thread_create(
        stack_buffer, THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    // Main thread can continue doing other tasks or simply sleep
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}
