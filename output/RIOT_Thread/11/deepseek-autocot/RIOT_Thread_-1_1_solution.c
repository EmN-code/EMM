
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

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
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize GPIO (example peripheral initialization)
    gpio_init(GPIO_PIN(0, 13), GPIO_OUT);

    // Create the receiver thread with higher priority
    receiver_pid = thread_create(
        thread_stack, RECEIVER_STACKSIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    // Create the sender thread with lower priority
    sender_pid = thread_create(
        thread_stack, SENDER_STACKSIZE,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    // Main thread can continue with other tasks or sleep
    while (1) {
        xtimer_sleep(5);
    }

    return 0;
}
